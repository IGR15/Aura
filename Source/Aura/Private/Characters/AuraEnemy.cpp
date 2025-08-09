// IGR1S


#include "Characters/AuraEnemy.h"

#include "AbilitySyste/AuraAbilitySystemComponent.h"
#include "AbilitySyste/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent=CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystem_Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighLightAcotr()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}

int32 AAuraEnemy::GetPlayerLevel()
{
	return level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfor();

	if (UAuraUserWidget* AuraUserWidget= Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		//now the enemy owns this widget controller
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* AuraAS =CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		// these callback lambdas broadcast when the attribute changes not initialize them 
		//we are binding to the ASC delegates because they broadcast when they change
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}

		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}

		);

		//these broadcasts are for initializing the attributes
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
	
}

void AAuraEnemy::InitAbilityActorInfor()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInofSet();
	
	InitializeDefaultAttributes();
}
