#include "SpellTargetSystemComponent.h"
#include "Components/DecalComponent.h"
#include "../Player/WADPlayerController.h"
#include "SpellTarget.h"

USpellTargetSystemComponent::USpellTargetSystemComponent()
{
}

void USpellTargetSystemComponent::StartSpellTargetSystem(float Radius)
{
	if (!SpellTargetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("The spell target class isn't set in the Spell Target System Component of the WAD Player Controller"));
	}
	SpellTarget = Cast<ASpellTarget>(GetWorld()->SpawnActor(SpellTargetClass));

	if (!SpellTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("Spell Target didnt spawn correctly"));
	}

	GetWorld()->GetTimerManager().SetTimer(SpellTargetHandle, this, &USpellTargetSystemComponent::SimulateSpellTarget, 0.03f, true, 0.0f);
}

 

void USpellTargetSystemComponent::StopSpellTargetSystem()
{
	GetWorld()->GetTimerManager().ClearTimer(SpellTargetHandle);
	SpellTarget->Destroy();
}

void USpellTargetSystemComponent::SimulateSpellTarget()
{
	FVector StartLocation;
	FVector StartDirection;

	OwningController->DeprojectMousePositionToWorld(StartLocation, StartDirection);

	FVector EndLocation = StartLocation + StartDirection * 5000.0f;
	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility);



	float MouseX;
	float MouseY;

	OwningController->GetMousePosition(MouseX, MouseY);

	OwningController->GetHitResultAtScreenPosition(FVector2D(MouseX, MouseY), ECC_Visibility, false, OutHit);


	SpellTarget->SetActorLocation(OutHit.ImpactPoint);
	FVector Location = SpellTarget->GetActorLocation();
	

	UE_LOG(LogTemp, Warning, TEXT("Spell Target position: X: %f Y: %f Z: %f"), Location.X, Location.Y, StartLocation.Z);


}
