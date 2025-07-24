// IGR1S


#include "Actor/AuraAffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


// Sets default values
AAuraAffectActor::AAuraAffectActor()
{
 	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent( CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}




void AAuraAffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AAuraAffectActor::ApplyEffictToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamePlayEffectClass)
{
	//you can do all of this in blueprint

	
	UAbilitySystemComponent* TargetAbilitySystemComponent= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAbilitySystemComponent==nullptr)return;
	check(GamePlayEffectClass);
	
	// a handel is a light wight wrapper that handles context and its data 
	FGameplayEffectContextHandle EffectContextHandle= TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	//this is also a Wrapper that holds the effectSpec
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GamePlayEffectClass,1.f,EffectContextHandle);
	
	TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
}



