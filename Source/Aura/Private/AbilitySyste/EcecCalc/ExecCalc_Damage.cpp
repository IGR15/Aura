// IGR1S


#include "AbilitySyste/EcecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySyste/AuraAttributeSet.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar=SourceASC? SourceASC->GetAvatarActor():nullptr;	
	const AActor* TargetAvatar=TargetASC? TargetASC->GetAvatarActor():nullptr;

	const FGameplayEffectSpec& Spec=ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTages=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	//get Damage set by caller magnitude
	float Damage=Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags=SourceTages;
	EvaluateParams.TargetTags=TargetTags;

	


	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
