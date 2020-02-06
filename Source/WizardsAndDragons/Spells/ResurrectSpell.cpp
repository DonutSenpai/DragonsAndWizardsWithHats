#include "ResurrectSpell.h"
#include "../Components/WADHealthComponent.h"
#include "../Player/WizardsAndDragonsCharacter.h"

void UResurrectSpell::InternalCastSpell(FVector TargetLocation)
{
	TArray<AActor*> OverlappedActors = GetSpellTargetsInRadius(TargetLocation);
	UE_LOG(LogTemp, Warning, TEXT("Resurrection spell happened"));

	if (OverlappedActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Resurrection spell is cast"));
		SpellTarget = OverlappedActors[0];
		GetWorld()->GetTimerManager().SetTimer(ResurrectHandle, this, &UResurrectSpell::Resurrect, TravelTime, false);

		OnCastSpellEffectInternal(TravelTime, SpellTarget);
	}

}

void UResurrectSpell::InternalDealDamage(const TArray<AActor*>& SpellTargets)
{
	if (Cast<AWizardsAndDragonsCharacter>(SpellTargets[0]))
	{
		if (UWADHealthComponent* HealthComp = SpellTargets[0]->FindComponentByClass<UWADHealthComponent>())
		{
			HealthComp->Resurrect();
		}
	}

	SpellTarget = nullptr;
}

void UResurrectSpell::Resurrect()
{
	if (SpellTarget)
	{
		TArray<AActor*> myArray;
		myArray.Add(SpellTarget);
		Server_DealDamage(myArray);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Spell Target, can't Resurrect damage."));
	}
}

void UResurrectSpell::OnCastSpellEffectInternal_Implementation(float TimeToTravel, AActor* TargetActor /*= nullptr*/)
{
	OnCastSpellEffect(TimeToTravel, TargetActor);
}

