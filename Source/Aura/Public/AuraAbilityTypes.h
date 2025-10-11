#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject>WorldContextObject=nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect>DamageGameplayEffectClass=nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>TargetASC;
	
	UPROPERTY()
	float BaseDamage=0.f;
	
	UPROPERTY()
	float AbilityLevel=1.f;
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float DebuffChance=0.f;
	
	UPROPERTY()
	float DebuffDamage=0.f;
	
	UPROPERTY()
	float DebuffDuration=0.f;
	
	UPROPERTY()
	float DebuffFrequency=0.f;
	
};
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
