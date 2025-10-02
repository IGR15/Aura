// IGR1S


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
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

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags=FAuraGameplayTags::Get();
	const int32 SpellPoints=GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid=AbilityTag.IsValid();
	const bool bTagNone=AbilityTag.MatchesTag(GameplayTags.Abilities_None);

	FGameplayAbilitySpec* AbilitySpec=GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	
	const  bool bSpecValid=AbilitySpec!=nullptr;
	if (!bTagValid|| bTagNone|| !bSpecValid)
	{
		AbilityStatus=GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus=GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}
	bool bEnableSpendPoints=false;
	bool bEnableEquip=false;
	ShouldEnableButtons(AbilityStatus,SpellPoints,bEnableSpendPoints,bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags=FAuraGameplayTags::Get();
	bShouldEnableSpellPointsButton=false;
	bShouldEnableEquipButton=false;


	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton=true;
		if (SpellPoints>0)
		{
			bShouldEnableSpellPointsButton=true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints>0)
		{
			bShouldEnableSpellPointsButton=true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton=true;
		if (SpellPoints>0)
		{
			bShouldEnableSpellPointsButton=true;
		}
	}
	

}
