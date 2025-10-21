// IGR1S


#include "AbilitySyste/Abilities/AuraFIreBolt.h"

#include "AbilitySyste/AuraAbilitySystemLibrary.h"
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

	TArray<FVector>Directions=UAuraAbilitySystemLibrary::EvenlyRotatedVectors(Forward,FVector::UpVector,ProjectileSpread,NumProjectiles);
	TArray<FRotator> Rotaions=UAuraAbilitySystemLibrary::EvenlySpaceDRotators(Forward,FVector::UpVector,ProjectileSpread,NumProjectiles);
}

