#include "SpellTargetSystemComponent.h"
#include "Components/DecalComponent.h"
#include "../Player/WADPlayerController.h"
#include "SpellTarget.h"
#include "../Spells/SpellBase.h"

USpellTargetSystemComponent::USpellTargetSystemComponent()
{
}


void USpellTargetSystemComponent::StartSpellTargetSystem(USpellBase* Spell)
{
	if (!SpellTargetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("The spell target class isn't set in the Spell Target System Component of the WAD Player Controller"));
	}

	if (Spell == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("You can't start the spell target system without a valid spell class as a parameter."));
	}

	if (SpellTarget)
	{
		StopSpellTargetSystem();
	}

	SelectedSpell = Spell;

	SpellTarget = Cast<ASpellTarget>(GetWorld()->SpawnActor(SpellTargetClass));
	if (SpellTarget)
	SpellTarget->SetRadius(SelectedSpell->Radius);

	SpellTargetRangeIndicator = Cast<ASpellTarget>(GetWorld()->SpawnActor(SpellTargetRangeIndicatorClass));
	if (SpellTargetRangeIndicator)
	SpellTargetRangeIndicator->SetRange(SelectedSpell->Range);

	if (SpellTarget)
	{
		bIsSystemActive = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Spell Target didnt spawn correctly"));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(SpellTargetHandle, this, &USpellTargetSystemComponent::SimulateSpellTarget, 0.03f, true, 0.0f);

}

void USpellTargetSystemComponent::StopSpellTargetSystem()
{
	GetWorld()->GetTimerManager().ClearTimer(SpellTargetHandle);

	if (SpellTarget)
	{
		SpellTarget->Destroy();
	}

	if (SpellTargetRangeIndicator)
	{
		SpellTargetRangeIndicator->Destroy();
	}

	bIsSystemActive = false;
	SelectedSpell = nullptr;
}

void USpellTargetSystemComponent::CastSpell()
{
	if (SpellTarget == nullptr || SelectedSpell->GetIsOnCooldown() || !CanCastSpell()) return;

	SelectedSpell->Server_CastSpell(SpellTarget->GetActorLocation());
	StopSpellTargetSystem();
	OwningController->OnSpellCast();

}

bool USpellTargetSystemComponent::GetIsSystemAlreadyActive(USpellBase* IsSpellActive)
{
	if (SelectedSpell)
	{

		return SelectedSpell->Name == IsSpellActive->Name;
	}

	return false;

}

bool USpellTargetSystemComponent::CanActivateSystem(USpellBase* SpellToActivate)
{
	if (SpellToActivate->GetIsOnCooldown())
		return false;

	if (SelectedSpell && SelectedSpell->Name == SpellToActivate->Name)
		return false;

	return true;
}

void USpellTargetSystemComponent::SimulateSpellTarget()
{

	float MouseX;
	float MouseY;
	OwningController->GetMousePosition(MouseX, MouseY);
	OwningController->GetHitResultAtScreenPosition(FVector2D(MouseX, MouseY), ECC_Visibility, false, OutHitMousePosition);


	SpellTarget->SetActorLocation(OutHitMousePosition.ImpactPoint);

	if (CanCastSpell())
	{
		SpellTarget->SetIsInRange(true);
		SpellTargetRangeIndicator->SetIsInRange(true);
	}
	else
	{
		SpellTarget->SetIsInRange(false);
		SpellTargetRangeIndicator->SetIsInRange(false);
	}

	FVector RangeIndicatorLocation = OwningController->GetPawn()->GetActorLocation();	
	RangeIndicatorLocation.Z -= 175.f;
	SpellTargetRangeIndicator->SetActorLocation(RangeIndicatorLocation);

}

bool USpellTargetSystemComponent::CanCastSpell()
{
	float DistanceToSpellTarget = FVector::Distance(OwningController->GetPawn()->GetActorLocation(), SpellTarget->GetActorLocation());

	if (DistanceToSpellTarget >= SelectedSpell->Range)
	{
		return false;
	}
	else
	{
		return true;
	}
}
