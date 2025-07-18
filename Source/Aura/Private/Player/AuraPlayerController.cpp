// IGR1S


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CurserTrace();
}
void AAuraPlayerController::CurserTrace()
{
	FHitResult CurserHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CurserHit);
	if (!CurserHit.bBlockingHit) return;
	LastActor=ThisActor;
	ThisActor=CurserHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *    - Do nothing
	 *
	 * B. LastActor is null && ThisActor is valid
	 *    - Highlight ThisActor
	 *
	 * C. LastActor is valid && ThisActor is null
	 *    - UnHighlight LastActor
	 *
	 * D. Both actors are valid, but LastActor != ThisActor
	 *    - UnHighlight LastActor, and Highlight ThisActor
	 *
	 * E. Both actors are valid, and are the same actor
	 *    - Do nothing
	 */
	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
		{
			//case B
			ThisActor->HighLightActor();
		}else
		{
			//case B the else is not necessary 
			
		}
	}
	else //last Actor is vaild
	{
		if (ThisActor==nullptr)
		{
			//case C
			LastActor->UnHighLightAcotr();
		}
		else //both are vaild
		{
			if (LastActor!=ThisActor)
			{
				//case D
				LastActor->HighLightActor();
				ThisActor->HighLightActor();
			}
			else //else is not necessary 
			{
				//case E
			}
		}
	}

}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);

	bShowMouseCursor = true;
	DefaultMouseCursor=EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;

	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
		
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this , &AAuraPlayerController::Move);

	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Get 2D input vector from the input action (X = Right/Left, Y = Forward/Backward)
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// Get the controller's current rotation (camera view direction)
	const FRotator Rotation= GetControlRotation();
	// Create a yaw-only rotator (ignore pitch and roll to keep movement horizontal)
	const FRotator YawRotator(0.f,Rotation.Yaw,0.f);
	
	// Get forward vector (X axis) from yaw-only rotation for forward/back movement
	const FVector ForwardDirection= FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	// Get the right vector (Y axis) from full rotation for strafing movement
	const FVector RightDirection= FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	// Move the pawn if it exists
	if (APawn* ControlledPawn {GetPawn<APawn>()})
	{
		// Apply movement in the forward direction scaled by Y input
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		// Apply movement in the right direction scaled by X input
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	// Processes movement input by converting 2D input into world-space directions.
	// Extracts forward and right vectors based on controller yaw and applies movement to the controlled pawn.

}


