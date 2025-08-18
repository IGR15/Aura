// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "AbilitySyste/Abilities/AuraGameplayAbility.h"
#include "AuraDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat>DamageTypes;
	
};
