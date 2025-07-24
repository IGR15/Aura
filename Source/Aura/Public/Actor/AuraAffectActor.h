// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAffectActor.generated.h"

class UGameplayEffect;
UCLASS()
class AURA_API AAuraAffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraAffectActor();
	UFUNCTION()

	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffictToActor(AActor* TargetActor,TSubclassOf<UGameplayEffect>GamePlayEffectClass);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InsantGampelayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGampelayEffectClass;
	
};
