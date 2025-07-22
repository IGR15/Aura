// IGR1S


#include "Actor/AuraAffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySyste/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
AAuraAffectActor::AAuraAffectActor()
{
 	PrimaryActorTick.bCanEverTick = false;
	Mesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	
}

void AAuraAffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* MutableSet=const_cast<UAuraAttributeSet*>(AttributeSet);
		MutableSet->SetHealth(AttributeSet->GetHealth()+25.f);

		Destroy();
	}
}

void AAuraAffectActor::EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}



void AAuraAffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this , &AAuraAffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this , &AAuraAffectActor::EndOverLap);
	
}



