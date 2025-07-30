// IGR1S


#include "AbilitySyste/MMC/MMC_MaxHealth.h"

#include "AbilitySyste/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture=UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot=false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather tags from source and target optional
	const FGameplayTagContainer* SourceTag= Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag= Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTag;
	EvaluateParameters.TargetTags=TargetTag;
	float Vigor=0.0f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Vigor);
	Vigor=FMath::Max<float>(Vigor,0.0f);
	//we can cast source object into an interface that way we can get the player level
	ICombatInterface* CombatInterface= Cast<ICombatInterface>(	Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel= CombatInterface->GetPlayerLevel();

	return 80.f+2.5f*Vigor+10.f*PlayerLevel;

}
