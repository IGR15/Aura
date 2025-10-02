// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature,bool,
	bSpendPointButtonEnabled,bool,bEquipButtonEnabled);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadCastInitValues() override;
	virtual void BindCallbacks() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangeSignature SpellPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus,int32 SpellPoints,bool& bShouldEnableSpellPointsButton,bool& bShouldEnableEquipButton);
	
};
