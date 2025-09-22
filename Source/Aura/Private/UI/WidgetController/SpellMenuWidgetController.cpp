// IGR1S


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySyste/AuraAbilitySystemComponent.h"
#include "AbilitySyste/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadCastInitValues()
{
	BroadCastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
	
}

void USpellMenuWidgetController::BindCallbacks()
{
	GetAuraASC()->AbilityStatusChangeDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag)
		{
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info=AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag=StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
	GetAuraPS()->OnSpellPointsChangeDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
	});
}
