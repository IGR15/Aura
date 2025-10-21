// IGR1S


#include "AbilitySyste/Abilities/AuraFIreBolt.h"

#include "Kismet/KismetSystemLibrary.h"

FString UAuraFIreBolt::GetDescription(int32 Level)
{
	
	const float ManaCost=FMath::Abs(GetManaCost(Level));

	
	int32 ScaledDamage =Damage.GetValueAtLevel(Level);
	const float Cooldown = GetCoolDown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
					// Title
					"<Title>FIRE BOLT</>\n\n"

					// Level
					"<Small>Level: </><Level>%d</>\n"
					// ManaCost
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"
					// Cooldown
					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
					"<Default>Launches a bolt of fire, "
					"exploding on impact and dealing: </>"

					// Damage
					"<Damage>%d</><Default> fire damage with"
					" a chance to burn</>"),

					// Values
					Level,
					ManaCost,
					Cooldown,
					ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
					// Title
					"<Title>FIRE BOLT</>\n\n"

					// Level
					"<Small>Level: </><Level>%d</>\n"
					// ManaCost
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"
					// Cooldown
					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

					// Number of FireBolts
					"<Default>Launches %d bolts of fire, "
					"exploding on impact and dealing: </>"

					// Damage
					"<Damage>%d</><Default> fire damage with"
					" a chance to burn</>"),

					// Values
					Level,
					ManaCost,
					Cooldown,
					FMath::Min(Level, NumProjectiles),
					ScaledDamage);
	}
}

FString UAuraFIreBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL: </>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
}

void UAuraFIreBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTage,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)return;
	const FVector SocketLocation=ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		SocketTage);
	//const FVector SocketLocation=CombatInterface->Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
	FRotator Rotation=(ProjectileTargetLocation - SocketLocation).Rotation();
		
	if (bOverridePitch) Rotation.Pitch+=PitchOverride;

	const FVector Forward=Rotation.Vector();
	const FVector LeftOfSpread=Forward.RotateAngleAxis(-ProjectileSpread/2.f,FVector::UpVector);
	const FVector RightOfSpreed=Forward.RotateAngleAxis(ProjectileSpread/2.f,FVector::UpVector);
	
	// NumProjectiles=FMath::Min(MaxNumProjectiles,GetAbilityLevel());
	if (NumProjectiles>1)
	{
		const float DeltaSpread=ProjectileSpread/(NumProjectiles-1);
		for (int32 i=0;i<NumProjectiles;i++)
		{
			const FVector Direction=LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				SocketLocation,
				SocketLocation+Direction*75.f,
				10.f,
				FLinearColor::Green,
				120,
				5
				);


		}
	}
	else
	{
		
	}
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation,SocketLocation+Rotation.Vector()*100.f,10.f,FLinearColor::Green,120,5);
}

