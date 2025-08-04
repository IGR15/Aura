// IGR1S


#include "UI/WidgetController/MenuWidgetController.h"
#include <utility>
#include "AbilitySyste/Data/AttributeInfo.h"
#include "AbilitySyste/AuraAttributeSet.h"

void UMenuWidgetController::BroadCastInitValues()
{
	UAuraAttributeSet* AuraAttributeSet= CastChecked<UAuraAttributeSet>(AttributeSet);
	check(Attributeinfo);
	
	for (auto& Pair :AuraAttributeSet->TagsToAttributes)
	{
		BroadCastAttrubteInfo(Pair.Key,Pair.Value());
	}
	
}

void UMenuWidgetController::BindCallbacks()
{
	UAuraAttributeSet* AS= CastChecked<UAuraAttributeSet>(AttributeSet);
	check(Attributeinfo);
	for (auto& Pair :AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair,AS](const FOnAttributeChangeData& Data)
			{
				BroadCastAttrubteInfo(Pair.Key,Pair.Value());
			}
		);
	}
}

void UMenuWidgetController::BroadCastAttribteInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)const
{
	FAuraAttributeInfo Info =Attributeinfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue=Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
	
}
