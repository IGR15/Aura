#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"
USTRUCT(BlueprintType)
struct FAuraGamePlayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGamePlayEffectContext* Duplicate() const
	{
		FAuraGamePlayEffectContext* NewContext = new FAuraGamePlayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	bool IsCriticalHit()const { return bIsCriticalHit;}
	bool IsBlockedHit()const { return bIsBlockedHit;}

	void SetIsCriticalHit(bool bInIsCriticalHit){bIsCriticalHit=bInIsCriticalHit;}
	void SetIsBlockedHit(bool bInIsBlockedHit){bIsBlockedHit=bInIsBlockedHit;}

protected:
	UPROPERTY()
	bool bIsBlockedHit=false;
	UPROPERTY()
	bool bIsCriticalHit=false;
	
	
};
template <>
struct TStructOpsTypeTraits<FAuraGamePlayEffectContext> :  TStructOpsTypeTraitsBase2<FAuraGamePlayEffectContext>
{
	enum
	{
		WithNetSerializer=true,
		WithCopy=true
	};
};
