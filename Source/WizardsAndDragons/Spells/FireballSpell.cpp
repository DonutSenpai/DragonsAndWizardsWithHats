#include "FireballSpell.h"
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>

void UFireballSpell::InternalCastSpell(FVector TargetLocation)
{
	OnCastSpellEffectInternal(TravelTime, TargetLocation);
	GetWorld()->GetTimerManager().SetTimer(HitExplosionHandle, FTimerDelegate::CreateUObject(this, &UFireballSpell::HitExplosion, TargetLocation), TravelTime, false);
}

void UFireballSpell::OnCastSpellEffectInternal_Implementation(float TimeToTravel, FVector TargetLocation /*= FVector::ZeroVector*/)
{
	OnCastSpellEffect(TravelTime, TargetLocation);

}

void UFireballSpell::HitExplosion(FVector TargetLocation)
{
	TArray<AActor*> ExplosionTargets = GetSpellTargetsInRadius(TargetLocation);
	Server_DealDamage(ExplosionTargets);

	if (ExplosionTargets.Num() > 0)
	{
		for (AActor* HitActor : ExplosionTargets)
		{
			if (ACharacter* HitCharacter = Cast <ACharacter>(HitActor))
			{
				//CALCULATE LAUNCH DIRECTION

				FVector UpLaunchVector = FVector(0.0f, 0.0f, Radius);
				FVector DirectionToTarget = HitCharacter->GetActorLocation() - TargetLocation;
				DirectionToTarget.Z = 0.0f;

				FVector LaunchDirection = DirectionToTarget + UpLaunchVector;
				LaunchDirection.Z += ExplosionLaunchHeightAdd;
				LaunchDirection.Normalize();			

					//----------------------
				
				FVector LaunchVelocity = LaunchDirection * ExplosionLaunchPower;
				HitCharacter->LaunchCharacter(LaunchVelocity, false, false);
			}
		}


	}
}
