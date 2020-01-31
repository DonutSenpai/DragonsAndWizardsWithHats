#include "DragonProjectile.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "../Player/WizardsAndDragonsCharacter.h"
#include "../Components/WADHealthComponent.h"

// Sets default values
ADragonProjectile::ADragonProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetReplicates(true);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
}

// Called when the game starts or when spawned
void ADragonProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(6.0f);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADragonProjectile::OnOverlapBegin);
}

void ADragonProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();

	if (HasAuthority())
	{		
			if (OtherActor->GetClass()->IsChildOf<AWizardsAndDragonsCharacter>())
			{
				UWADHealthComponent* PlayerHealthComp = OtherActor->FindComponentByClass<UWADHealthComponent>();

				if (PlayerHealthComp)
				{
					PlayerHealthComp->DecreaseHealth(DamageAmmount, this);
				}
			}
			
	}
}

// Called every frame
void ADragonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(GetActorForwardVector() * ProjectileVelocity * DeltaTime, false, nullptr, ETeleportType::TeleportPhysics);
}

void ADragonProjectile::Explode()
{
	SetActorTickEnabled(false);
	BP_OnExplode();
	Destroy();
}

