// IGR1S


#include "AbilitySyste/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySyste/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInofSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbilities)
	{
		
		FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(AbilityClass,1.f);
		if (const UAuraGameplayAbility* AuraAbility=Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			FGameplayTagContainer& DynamicTag=AbilitySpec.GetDynamicSpecSourceTags();
			DynamicTag.AddTag(AuraAbility->StartUpInputTag);
			GiveAbility(AbilitySpec);
		}
		
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InpuTag)
{
	if (!InpuTag.IsValid())return;

	for (auto& AbilitySpec :GetActivatableAbilities())
	{
		FGameplayTagContainer& DynamicTag=AbilitySpec.GetDynamicSpecSourceTags();
		if (DynamicTag.HasTagExact(InpuTag))
		{
			AbilitySpecInputPressed(AbilitySpec);//this checks if the ability input is pressed
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InpuTag)
{
	if (!InpuTag.IsValid())return;
	for (auto& AbilitySpec :GetActivatableAbilities())
	{
		FGameplayTagContainer& DynamicTag=AbilitySpec.GetDynamicSpecSourceTags();
		if (DynamicTag.HasTagExact(InpuTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer TagContainer;
	SpecApplied.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
	
}
