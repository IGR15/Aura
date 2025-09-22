// IGR1S


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySyste/AuraAbilitySystemComponent.h"
#include "AbilitySyste/Data/AbilityInfo.h"

void USpellMenuWidgetController::BroadCastInitValues()
{
	BroadCastAbilityInfo();
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
}
