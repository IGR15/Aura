// IGR1S


#include "AbilitySyste/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj=NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	APlayerController* PC= Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CurserHit;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,CurserHit);
	ValidData.Broadcast(CurserHit.Location);
}
