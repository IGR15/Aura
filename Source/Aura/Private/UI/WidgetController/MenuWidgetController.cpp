// IGR1S


#include "UI/WidgetController/MenuWidgetController.h"
#include "AbilitySyste/Data/AttributeInfo.h"
#include "AbilitySyste/AuraAttributeSet.h"
#include "AuraGameplayTags.h"

void UMenuWidgetController::BroadCastInitValues()
{
	UAuraAttributeSet* AuraAttributeSet= CastChecked<UAuraAttributeSet>(AttributeSet);
	check(Attributeinfo);
	FAuraAttributeInfo Info= Attributeinfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue=AuraAttributeSet->GetStrength();
	
	AttributeInfoDelegate.Broadcast(Info);
}

void UMenuWidgetController::BindCallbacks()
{
	
}
