// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Characters/AuraCharacterBase.h"
#include "AbilitySyste/Data/CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
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
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
protected:
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfor()override;
	
	virtual void InitializeDefaultAttributes()const override;


	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="charcter class defaults")
	int32 level=1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="charcter class defaults")
	ECharacterClass CharacterClass=ECharacterClass::Warrior;	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>HealthBar;
	
};
