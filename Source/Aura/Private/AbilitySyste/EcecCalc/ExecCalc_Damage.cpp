// IGR1S


#include "AbilitySyste/EcecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySyste/AuraAbilitySystemLibrary.h"
#include "AbilitySyste/AuraAttributeSet.h"
#include "AbilitySyste/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

//Raw Struct To capture attributes
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
	}
};
static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}
UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar=SourceASC? SourceASC->GetAvatarActor():nullptr;	
	AActor* TargetAvatar=TargetASC? TargetASC->GetAvatarActor():nullptr;

	ICombatInterface* SourceCombatInterface=Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargeteCombatInterface=Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec=ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTages=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags=SourceTages;
	EvaluateParams.TargetTags=TargetTags;

	//get Damage set by caller magnitude
	float Damage=Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	//Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParams,TargetBlockChance);
	TargetBlockChance=FMath::Max<float>(TargetBlockChance,0.f);

	// if block, halve the damage
	const bool bBlocked=FMath::RandRange(1,100)<TargetBlockChance;
	Damage=bBlocked ? Damage / 2.f : Damage;
	
	//ArmorPenetration Ignores a percentage of the target's armor 
	float TargetArmor=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParams,TargetArmor);
	TargetArmor=FMath::Max<float>(TargetArmor,0.f);

	float SourceArmorPenetration=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParams,SourceArmorPenetration);
	SourceArmorPenetration=FMath::Max<float>(SourceArmorPenetration,0.f);
	const UCharacterClassInfo* CharacterClassInfo=UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve= CharacterClassInfo->DamagecalculationCoeffficient->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient= ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	
	//Armor Ignores a percentage of incoming Damage.
	const float EffectiveArmor=TargetArmor *(100-SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	const FRealCurve*EffectiveArmorCoefficientCurve=CharacterClassInfo->DamagecalculationCoeffficient->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient=EffectiveArmorCoefficientCurve->Eval(TargeteCombatInterface->GetPlayerLevel());
	
	Damage*=(100-EffectiveArmor*EffectiveArmorCoefficient)/100.f;

	float SourceCriticalHitChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParams,SourceCriticalHitChance);
	SourceCriticalHitChance=FMath::Max<float>(SourceCriticalHitChance,0.f);

	float TargetCriticalHitResistance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParams,TargetCriticalHitResistance);
	TargetCriticalHitResistance=FMath::Max<float>(TargetCriticalHitResistance,0.f);

	float SourceCriticalHitDamage=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParams,SourceCriticalHitDamage);
	SourceCriticalHitDamage=FMath::Max<float>(SourceCriticalHitDamage,0.f);

	const FRealCurve*EffectiveCriticalHitResistanceCurve=CharacterClassInfo->DamagecalculationCoeffficient->FindCurve(FName("CriticalHitResistance"),FString());
	const float EffectiveCriticalHitResistanceCoefficient=EffectiveCriticalHitResistanceCurve->Eval(TargeteCombatInterface->GetPlayerLevel());

	const float EffectiveSourceCriticalHitChance=SourceCriticalHitChance-TargetCriticalHitResistance*EffectiveCriticalHitResistanceCoefficient;
	const bool bCriticalHit=FMath::RandRange(1,100)< EffectiveSourceCriticalHitChance;
	Damage=bCriticalHit ? 2.f*Damage+SourceCriticalHitDamage : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
