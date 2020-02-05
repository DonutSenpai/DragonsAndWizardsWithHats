// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WizardsAndDragonsCharacter.generated.h"

UCLASS(config=Game)
class AWizardsAndDragonsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AWizardsAndDragonsCharacter();

	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY()
	bool RotateCamera = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpellSlots)
	TArray<class USpellBase*> SpellSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWADHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsCastingSpell = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpellCast();

	UFUNCTION(NetMulticast, Reliable)
	void InternalOnSpellCast();

protected:

	//-----INPUT STUFF-------
	UFUNCTION(BlueprintCallable, Category = "WAD Input")
	void SetInputEnabled();

	UFUNCTION(BlueprintCallable, Category = "WAD Input")
	void SetInputDisabled();

	bool InputEnabled = true;
	//-----------------------


	void Turn(float Rate);
	void LookUp(float Rate);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = Hats)
	void SetHat(int HatNumber);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetHat(int HatNumber);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

