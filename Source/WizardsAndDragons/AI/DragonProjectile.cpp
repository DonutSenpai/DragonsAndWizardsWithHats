#include "DragonProjectile.h"
#include "Engine/World.h"

// Sets default values
ADragonProjectile::ADragonProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ADragonProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(6.0f);
}

// Called every frame
void ADragonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FHitResult Hit;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		const FVector EndLocation = GetActorLocation() + GetActorForwardVector() * 300.0f;
		GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), EndLocation, ECC_Visibility, CollisionParams);

		if (Hit.bBlockingHit || Hit.Actor.IsValid())
		{
			Explode();
		}
	}

	AddActorWorldOffset(GetActorForwardVector() * ProjectileVelocity * DeltaTime, false, nullptr, ETeleportType::TeleportPhysics);
}

void ADragonProjectile::Explode()
{
	SetActorTickEnabled(false);
	BP_OnExplode();
}

