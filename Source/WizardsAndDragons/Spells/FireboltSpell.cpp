#include "FireboltSpell.h"
#include <Kismet/KismetSystemLibrary.h>



void UFireboltSpell::Server_CastSpell_Implementation(FVector TargetLocation)
{
	Super::Server_CastSpell(TargetLocation);

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
	else
	{
		SpellTarget = OverlappedActors[0];
	}



}
