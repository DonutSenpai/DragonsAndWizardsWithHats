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

	SelectedSpell = Spell;

	SpellTarget = Cast<ASpellTarget>(GetWorld()->SpawnActor(SpellTargetClass));
	SpellTarget->SetRadius(SelectedSpell->Radius);

	if (SpellTarget)
	{
		bIsSystemActive = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Spell Target didnt spawn correctly"));
	}

	GetWorld()->GetTimerManager().SetTimer(SpellTargetHandle, this, &USpellTargetSystemComponent::SimulateSpellTarget, 0.03f, true, 0.0f);

}

void USpellTargetSystemComponent::StopSpellTargetSystem()
{
	GetWorld()->GetTimerManager().ClearTimer(SpellTargetHandle);
	SpellTarget->Destroy();
	bIsSystemActive = false;
}

void USpellTargetSystemComponent::CastSpell()
{
	if (SpellTarget == nullptr || SelectedSpell->GetIsOnCooldown()) return;

	SelectedSpell->Server_CastSpell(SpellTarget->GetActorLocation());
	StopSpellTargetSystem();

}

void USpellTargetSystemComponent::SimulateSpellTarget()
{

	float MouseX;
	float MouseY;
	OwningController->GetMousePosition(MouseX, MouseY);
	OwningController->GetHitResultAtScreenPosition(FVector2D(MouseX, MouseY), ECC_Visibility, false, OutHitMousePosition);


	SpellTarget->SetActorLocation(OutHitMousePosition.ImpactPoint);

//	FVector Location = SpellTarget->GetActorLocation();
	

	//UE_LOG(LogTemp, Warning, TEXT("Spell Target position: X: %f Y: %f Z: %f"), Location.X, Location.Y, StartLocation.Z);


}
