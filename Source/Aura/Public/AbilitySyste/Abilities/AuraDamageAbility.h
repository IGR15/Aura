// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySyste/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor=nullptr)const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")

	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffChance=20.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffDamage=5.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffFrequency=1.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffDuration=5.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const ;
	
	
};
