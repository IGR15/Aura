// IGR1S


#include "AbilitySyste/AuraAbilitySystemLibrary.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
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


	AActor* AvatarActor=ASC->GetAvatarActor();

	FGameplayEffectContextHandle PrimaryContextHandle= ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo)return;
	FCharacterClassDefaultInfo ClassDefaultInfo=CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	FGameplayEffectSpecHandle PrimarySpecHandle= ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,level,PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle= ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondarySpecHandle= ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,level,SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle= ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalSpecHandle= ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,level,VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilites(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo)return;
	for (TSubclassOf<UGameplayAbility> AbilityClass:	CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo= CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	for (auto AbilityClass: DefaultInfo.StartupAbilities)
	{
		
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			
			FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);

		}
	}
	
}
int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo)return 0;

	const FCharacterClassDefaultInfo& Info= CharacterClassInfo->GetCharacterClassInfo(CharacterClass);

	const float XPReward=Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* GameMode= Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode==nullptr)return nullptr;
	
	return GameMode->CharacterClassInfo;
	
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGamePlayEffectContext* AuraEffectContext=static_cast<const FAuraGamePlayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	//this is a regular c++ static cast we have to add the * to the pointer if it was an engine cast we dont 
	if (const FAuraGamePlayEffectContext* AuraEffectContext=static_cast<const FAuraGamePlayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
	
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	

	const bool bBothArePlayers=FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemy=FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));

	const bool bFriends=bBothArePlayers || bBothAreEnemy;

	return !bFriends;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockHit)
{
	if (FAuraGamePlayEffectContext* AuraEffectContext=static_cast<FAuraGamePlayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockHit);
	}
	
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FAuraGamePlayEffectContext* AuraEffectContext=static_cast<FAuraGamePlayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithInRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()));
			}
		}
	}
}


