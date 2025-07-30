// IGR1S


#include "AbilitySyste/MMC/MMC_MaxMana.h"
#include "AbilitySyste/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxMana::UMMC_MaxMana()
{
	InelligenceDef.AttributeToCapture=UAuraAttributeSet::GetIntelligenceAttribute();
	InelligenceDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	InelligenceDef.bSnapshot=false;

	RelevantAttributesToCapture.Add(InelligenceDef);
}
float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather tags from source and target optional
	const FGameplayTagContainer* SourceTag= Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag= Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTag;
	EvaluateParameters.TargetTags=TargetTag;
	float Inelligenc=0.0f;
	GetCapturedAttributeMagnitude(InelligenceDef,Spec,EvaluateParameters,Inelligenc);
	Inelligenc=FMath::Max<float>(Inelligenc,0.0f);
	//we can cast source object into an interface that way we can get the player level
	ICombatInterface* CombatInterface= Cast<ICombatInterface>(	Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel= CombatInterface->GetPlayerLevel();

	return 30.f+2.5f*Inelligenc+5.f*PlayerLevel;
}


