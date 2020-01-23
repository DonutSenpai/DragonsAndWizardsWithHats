#include "FireboltSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Components/WADHealthComponent.h"


void UFireboltSpell::InternalCastSpell(FVector TargetLocation)
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get UWorld, it is nullptr."));
		return;
	}

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(World, TargetLocation, Radius, ObjectTypes, nullptr, IgnoredActors, OverlappedActors);

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

	World->GetTimerManager().SetTimer(DealDamageHandle, this, &UFireboltSpell::DealDamage, 0.1f, false, TravelTime);
	OnCastSpellEffect(TravelTime, TargetLocation, SpellTarget);

}

void UFireboltSpell::InternalDealDamage(FVector TargetLocation)
{
	if (UWADHealthComponent* HealthComponent = SpellTarget->FindComponentByClass<UWADHealthComponent>())
	{
		HealthComponent->DecreaseHealth(Damage, GetOwner());
		UE_LOG(LogTemp, Warning, TEXT("Dealt damage"));
	}

	SpellTarget = nullptr;
}

void UFireboltSpell::DealDamage()
{
	UE_LOG(LogTemp, Error, TEXT("DEAL DAMAGE EVENT HAPPENED"));
	if (SpellTarget)
	{
		Server_DealDamage(FVector::ZeroVector);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Spell Target, can't deal damage."));
	}
}
