// IGR1S


#include "AbilitySyste/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location=GetAvatarActorFromActorInfo()->GetActorLocation();

	const FVector LeftOfSpread=Forward.RotateAngleAxis(SpawnSpread/2.f,FVector::UpVector);

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		Location,
		Location+LeftOfSpread*MaxSpawnDistance,
		4.f,
		FLinearColor::Green,
		3.f

		);

	const FVector RightOfSpread=Forward.RotateAngleAxis(-SpawnSpread/2.f,FVector::UpVector);

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		Location,
		Location+RightOfSpread*MaxSpawnDistance,
		4.f,
		FLinearColor::Blue,
		3.f

		);

	return TArray<FVector>();
}
