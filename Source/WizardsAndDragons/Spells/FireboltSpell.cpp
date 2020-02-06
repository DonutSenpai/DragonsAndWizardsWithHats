#include "FireboltSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Components/WADHealthComponent.h"


void UFireboltSpell::InternalCastSpell(FVector TargetLocation)
{
	TArray<AActor*> OverlappedActors = GetSpellTargetsInRadius(TargetLocation);
	UE_LOG(LogTemp, Warning, TEXT("Spell Targets in Radius: %d"), OverlappedActors.Num());

	if (OverlappedActors.Num() > 1)
	{
		float DistancesFromCenter[10] = { Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f, Radius * 2.f };

		for (int i = 0; i < OverlappedActors.Num(); i++)
		{
			if (i >= 10) break;
			
			DistancesFromCenter[i] = FVector::DistXY(OverlappedActors[i]->GetActorLocation(), TargetLocation);
		}

		int ClosestActorIndex = 0;

		for (int i = 0; i < 10; i++)
		{
			if (DistancesFromCenter[i] < DistancesFromCenter[ClosestActorIndex])
			{
				ClosestActorIndex = i;
			}
		}

		SpellTarget = OverlappedActors[FMath::Clamp(ClosestActorIndex, 0, OverlappedActors.Num())];
		
	}
	else if (OverlappedActors.Num() == 1)
	{
		SpellTarget = OverlappedActors[0];
	}

	GetWorld()->GetTimerManager().SetTimer(DealDamageHandle, this, &UFireboltSpell::DealDamage, TravelTime, false);
	OnCastSpellEffectInternal(TravelTime, TargetLocation, SpellTarget);

}

void UFireboltSpell::InternalDealDamage(const TArray<AActor*> &SpellTargets)
{
	if (UWADHealthComponent* HealthComponent = SpellTargets[0]->FindComponentByClass<UWADHealthComponent>())
	{
		HealthComponent->DecreaseHealth(Damage, GetOwner());
	}

	SpellTarget = nullptr;
}

void UFireboltSpell::OnCastSpellEffectInternal_Implementation(float TimeToTravel, FVector TargetLocation /*= FVector::ZeroVector*/, AActor* TargetActor /*= nullptr*/)
{
	OnCastSpellEffect(TimeToTravel, TargetLocation, TargetActor);
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
