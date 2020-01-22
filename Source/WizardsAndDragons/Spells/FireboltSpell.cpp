#include "FireboltSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Components/WADHealthComponent.h"



void UFireboltSpell::Server_CastSpell_Implementation(FVector TargetLocation)
{
	Super::Server_CastSpell(TargetLocation);


	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TargetLocation, Radius, ObjectTypes, nullptr, IgnoredActors, OverlappedActors);

	if (OverlappedActors.Num() > 1)
	{
		float DistanceFromCenter = Radius;

		for (AActor* OverlappedActor : OverlappedActors)
		{
			float ActorDistanceFromCenter = FVector::Distance(OverlappedActor->GetActorLocation(), TargetLocation);

			if (ActorDistanceFromCenter < DistanceFromCenter)
			{
				DistanceFromCenter = ActorDistanceFromCenter;

				SpellTarget = OverlappedActor;
			}
		}
	}
	else
	{
		SpellTarget = OverlappedActors[0];
	}

	GetWorld()->GetTimerManager().SetTimer(DealDamageHandle, this, &UFireboltSpell::DealDamage, 0.0f, false, TravelTime);
	OnCastSpellEffect(SpellTarget, TravelTime);
}

void UFireboltSpell::Server_DealDamage_Implementation(FVector TargetLocation)
{
	if (UWADHealthComponent* HealthComponent = SpellTarget->FindComponentByClass<UWADHealthComponent>())
	{
		HealthComponent->DecreaseHealth(Damage);
	}

	SpellTarget = nullptr;
}

void UFireboltSpell::DealDamage()
{
	if (SpellTarget)
	{
		Server_DealDamage(FVector::ZeroVector);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Spell Target, can't deal damage."));
	}
}
