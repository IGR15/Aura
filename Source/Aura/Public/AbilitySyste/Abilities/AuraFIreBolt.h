// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "AbilitySyste/Abilities/AuraProjectileSpell.h"
#include "AuraFIreBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFIreBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
