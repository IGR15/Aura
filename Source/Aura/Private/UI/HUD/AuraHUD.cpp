// IGR1S


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController=NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("overlay widget class uninitialized , please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass,TEXT("overlay widget controller class uninitialized , please fill out BP_AuraHUD "))
	
	UUserWidget* Widget= CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget=Cast<UAuraUserWidget>(Widget);
	const FWidgetControllerParams WCParams(PC,PS,ASC,AS);

	UOverlayWidgetController* WidgetController=GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(WidgetController);
	Widget->AddToViewport();
	
}


