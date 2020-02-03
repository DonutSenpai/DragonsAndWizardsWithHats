#include "FireStorm.h"
#include "../Components/WADHealthComponent.h"
#include "../Player/WizardsAndDragonsCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFireStorm::AFireStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AFireStorm::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFireStorm::OnOverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AFireStorm::OnOverlapEnd);


	GetWorld()->GetTimerManager().SetTimer(FireStormActivationTimer, this, &AFireStorm::ActivateFireStorm, InitialDelay, false);
}

void AFireStorm::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf<AWizardsAndDragonsCharacter>())
	{
		AffectedActors.Add(OtherActor);
	}
}

void AFireStorm::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AffectedActors.Remove(OtherActor);
}

void AFireStorm::DealDamage()
{
	for (auto& HitActor : AffectedActors)
	{
		UWADHealthComponent* HealthComp = HitActor->FindComponentByClass<UWADHealthComponent>();

		if (HealthComp)
		{
			HealthComp->DecreaseHealth(DamageAmmount, this);
		}
	}
}

void AFireStorm::ActivateFireStorm()
{
	bFireStormActive = true;

	GetWorld()->GetTimerManager().SetTimer(DealDamageTimerHandle, this, &AFireStorm::DealDamage, DamageInterval, true);
}

// Called every frame
void AFireStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

