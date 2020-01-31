#include "WADPlayerController.h"
#include "WizardsAndDragonsCharacter.h"
#include "../SpellTargetSystem/SpellTargetSystemComponent.h"
#include "../Spells/SpellBase.h"

AWADPlayerController::AWADPlayerController()
{
	SpellTargetSystem = CreateDefaultSubobject<USpellTargetSystemComponent>(TEXT("SpellTargetSystemComponent"));
}

void AWADPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RotateCamera", IE_Pressed, this, &AWADPlayerController::SetRotateCameraTrue);
	InputComponent->BindAction("RotateCamera", IE_Released, this, &AWADPlayerController::SetRotateCameraFalse);
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AWADPlayerController::PrintLeftClick);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AWADPlayerController::CastSpell);

}

void AWADPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	OwnedCharacter = Cast<AWizardsAndDragonsCharacter>(GetCharacter());
	SpellTargetSystem->OwningController = this;

}

void AWADPlayerController::SetRotateCameraTrue()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Rotate"));
	OwnedCharacter->RotateCamera = true;
	bShowMouseCursor = false;
	GetMousePosition(PreviousMouseX, PreviousMouseY);
}

void AWADPlayerController::SetRotateCameraFalse()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Stop Rotate"));

	OwnedCharacter->RotateCamera = false;
	SetMouseLocation(PreviousMouseX, PreviousMouseY);
	bShowMouseCursor = true;

}

void AWADPlayerController::StartSpellTargetSystem(class USpellBase* Spell)
{
	SpellTargetSystem->StartSpellTargetSystem(Spell);
}

void AWADPlayerController::OnSpellCast()
{
	OwnedCharacter->InternalOnSpellCast();
}

void AWADPlayerController::PrintLeftClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Left Clicked"));
}

void AWADPlayerController::CastSpell()
{
	//SpellTargetSystem->CastSpell();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Left Released"));
}

