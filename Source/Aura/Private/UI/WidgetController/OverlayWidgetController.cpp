// IGR1S


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySyste/AuraAbilitySystemComponent.h"
#include "AbilitySyste/AuraAttributeSet.h"
#include "AbilitySyste/Data/AbilityInfo.h"
#include "AbilitySyste/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadCastInitValues()
{
	// Force-cast the generic AttributeSet to your custom AuraAttributeSet
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Broadcast initial health values to binded UMG widgets
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacks()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	// Cast base attribute set to the specific AuraAttributeSet subclass
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AuraPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		}
		);

	// Subscribe Health attribute changes to OnHealthChanged delegate
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// Subscribe MaxHealth attribute changes to OnMaxHealthChanged delegate
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
			);
	

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
			);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
			
			
		);
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(AuraASC);
		}
		else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::OnInitializeStartupAbilities);

		}
		AuraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTages /* this is the input parameter*/ )
			{
				for (const FGameplayTag& Tag : AssetTages)
				{
					FGameplayTag MessageTag =FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row= GetDataTableRowByTag<FUIWidgetRow>(MessageDataTable,Tag);
						MessageWidgetRowDelegat.Broadcast(*Row);
					}
				}
			}
		);
	}

	
	
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)return;

	FForEachAbility BroadCastDelegate;
	BroadCastDelegate.BindLambda([this,AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info= AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag= AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	AuraAbilitySystemComponent->ForEachAbility(BroadCastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXp)const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo =AuraPlayerState->LevelUpInfo;

	checkf(LevelUpInfo,TEXT("Unabled to find levelupinfo. fill out auraplayerstate Blueprint"));

	const int32 Level=LevelUpInfo->FindLevelForXP(NewXp);
	const int32 MaxLevel=LevelUpInfo->LevelUpInfo.Num()-1;

	if (Level<=MaxLevel&&Level>0)
	{

		const int32 LevelUpReq=LevelUpInfo->LevelUpInfo[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement=LevelUpInfo->LevelUpInfo[Level-1].LevelUpRequirement;

		const int32 DeltaLevelReq=LevelUpReq-PreviousLevelUpRequirement;
		const int32 XPForThisLevel=NewXp- PreviousLevelUpRequirement;

		const float XPBarPercent=static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelReq);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
		
	}

}

