// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:

	AAuraEnemy();

	//Enemy Interface
	
	virtual void HighLightActor() override;
	virtual void UnHighLightAcotr() override;

	/**CombatInterface */
	virtual int32 GetPlayerLevel() override;
	/** EndCombatIterface*/
protected:
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfor()override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="charcter class defaults")
	int32 level=1;

	
	
};
