// IGR1S

#pragma once

#include "CoreMinimal.h"
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
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat>DamageTypes;


	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const ;
	
};
