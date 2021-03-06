// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WizardsAndDragonsCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../SpellTargetSystem/SpellTargetSystemComponent.h"
#include "WADPlayerController.h"
#include "../Components/WADHealthComponent.h"

//////////////////////////////////////////////////////////////////////////
// AWizardsAndDragonsCharacter

AWizardsAndDragonsCharacter::AWizardsAndDragonsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	HealthComponent = CreateDefaultSubobject<UWADHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);

}

void AWizardsAndDragonsCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		Multicast_SetHat(FMath::RandRange(0, 1));
	}

}

void AWizardsAndDragonsCharacter::InternalOnSpellCast_Implementation()
{
	OnSpellCast();
}

void AWizardsAndDragonsCharacter::Multicast_SetHat_Implementation(int HatNumber)
{
	SetHat(HatNumber);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWizardsAndDragonsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	if (!IsLocallyControlled()) return;
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWizardsAndDragonsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWizardsAndDragonsCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AWizardsAndDragonsCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AWizardsAndDragonsCharacter::LookUp);

}

void AWizardsAndDragonsCharacter::SetInputEnabled()
{
	InputEnabled = true;
}

void AWizardsAndDragonsCharacter::SetInputDisabled()
{
	InputEnabled = false;
}

void AWizardsAndDragonsCharacter::Turn(float Rate)
{
	if (RotateCamera)
	{
		AddControllerYawInput(Rate);
	}
}

void AWizardsAndDragonsCharacter::LookUp(float Rate)
{
	if (RotateCamera)
	{
		AddControllerPitchInput(Rate);
	}
}

void AWizardsAndDragonsCharacter::MoveForward(float Value)
{
	if (!InputEnabled) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWizardsAndDragonsCharacter::MoveRight(float Value)
{
	if (!InputEnabled) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


