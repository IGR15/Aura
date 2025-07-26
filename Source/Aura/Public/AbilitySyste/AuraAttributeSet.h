// IGR1S

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include  "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS_BASIC(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	FEffectProperties();
	UPROPERTY()
	UAbilitySystemComponent* SourceASC=nullptr;
	UPROPERTY()
	UAbilitySystemComponent* TargetAsc=nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor=nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor=nullptr;
	UPROPERTY()
	AController* TargetController=nullptr;
	UPROPERTY()
	AController* SourceController=nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter=nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter=nullptr;
	
	FGameplayEffectContextHandle EffectContextHandle;
	
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	//this is called Boiler Plates
	UPROPERTY(BlueprintReadOnly,Category="Vital Aattributes",ReplicatedUsing= OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet,Health);
	

	UPROPERTY(BlueprintReadOnly,Category="Vital Aattributes",ReplicatedUsing= OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet,MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,Category="Vital Aattributes",ReplicatedUsing= OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet,Mana);
	
	UPROPERTY(BlueprintReadOnly,Category="Vital Aattributes",ReplicatedUsing= OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet,MaxMana);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Properties) const ;
	
};
