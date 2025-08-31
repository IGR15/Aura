// IGR1S


#include "AbilitySyste/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location=GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread=SpawnSpread/NumMinions;
	const FVector RightOfSpread=Forward.RotateAngleAxis(SpawnSpread/2.f,FVector::UpVector);
	const FVector LeftOfSpread=Forward.RotateAngleAxis(-SpawnSpread/2.f,FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int i = 0; i < NumMinions; i++)
	{
		const FVector Direction=LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
		const FVector ChosenLocation=Location+Direction*FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
		SpawnLocations.Add(ChosenLocation);
		DrawDebugSphere(GetWorld(),ChosenLocation,20.f,12,FColor::Cyan,false,3.f);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		Location,
		Location+Direction*MaxSpawnDistance,
		4.f,
		FLinearColor::Green,
		3.f

		);
		DrawDebugSphere(GetWorld(),Location+Direction*MinSpawnDistance,10.f,12,FColor::Red,false,3.f);
		DrawDebugSphere(GetWorld(),Location+Direction*MaxSpawnDistance,10.f,12,FColor::Red,false,3.f);

	}

	return SpawnLocations;
}
