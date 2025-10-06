// IGR1S


#include "AbilitySyste/Abilities/AuraDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle= MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.0f);
	for (auto Pair : DamageTypes)
	{
		const float ScaleDamage=Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,ScaleDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FTaggedMontage UAuraDamageAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const 
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection =  FMath::RandRange(0,TaggedMontages.Num()-1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}

float UAuraDamageAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbility [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}
