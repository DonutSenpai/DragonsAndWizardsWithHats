#include "FireboltSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Components/WADHealthComponent.h"


void UFireboltSpell::InternalCastSpell(FVector TargetLocation)
{

	TArray<AActor*> OverlappedActors = GetSpellTargetsInRadius(TargetLocation);

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
	else if (OverlappedActors.Num() == 1)
	{
		SpellTarget = OverlappedActors[0];
	}

	GetWorld()->GetTimerManager().SetTimer(DealDamageHandle, this, &UFireboltSpell::DealDamage, TravelTime, false);
	OnCastSpellEffect(TravelTime, TargetLocation, SpellTarget);

}

void UFireboltSpell::InternalDealDamage(const TArray<AActor*> &SpellTargets)
{
	if (UWADHealthComponent* HealthComponent = SpellTargets[0]->FindComponentByClass<UWADHealthComponent>())
	{
		HealthComponent->DecreaseHealth(Damage, GetOwner());
	}

	SpellTarget = nullptr;
}

void UFireboltSpell::DealDamage()
{
	if (SpellTarget)
	{
		TArray<AActor*> myArray;
		myArray.Add(SpellTarget);
		Server_DealDamage(myArray);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Spell Target, can't deal damage."));
	}
}
