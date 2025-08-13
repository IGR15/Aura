// IGR1S


#include "AbilitySyste/EcecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
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

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags=SourceTages;
	EvaluateParams.TargetTags=TargetTags;
	float Armor=0.f;
	++Armor;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParams,Armor);
	Armor=FMath::Max<float>(0.f,Armor);

	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty,EGameplayModOp::Additive,Armor);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
