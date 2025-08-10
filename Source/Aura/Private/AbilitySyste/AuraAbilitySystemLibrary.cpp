// IGR1S


#include "AbilitySyste/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHUD=Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PS =PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC =PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController,PS,ASC,AS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHUD=Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PS =PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC =PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController,PS,ASC,AS);
			return AuraHUD->GetMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float level,UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* GameMode= Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode==nullptr)return;

	UCharacterClassInfo* CharacterClassInfo=GameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo=CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	
	FGameplayEffectSpecHandle PrimarySpecHandle= ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,level,ASC->MakeEffectContext());
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectSpecHandle SecondarySpecHandle= ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,level,ASC->MakeEffectContext());
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectSpecHandle VitalSpecHandle= ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,level,ASC->MakeEffectContext());
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}
