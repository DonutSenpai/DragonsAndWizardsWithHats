#include "WADPlayerController.h"
#include "WizardsAndDragonsCharacter.h"

AWADPlayerController::AWADPlayerController()
{

}

void AWADPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RotateCamera", IE_Pressed, this, &AWADPlayerController::SetRotateCameraTrue);
	InputComponent->BindAction("RotateCamera", IE_Released, this, &AWADPlayerController::SetRotateCameraFalse);
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AWADPlayerController::PrintLeftClick);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AWADPlayerController::PrintLeftRelease);

}

void AWADPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	OwnedCharacter = Cast<AWizardsAndDragonsCharacter>(GetCharacter());
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

void AWADPlayerController::PrintLeftClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Left Clicked"));
}

void AWADPlayerController::PrintLeftRelease()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Left Released"));
}

