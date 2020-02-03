// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireStorm.generated.h"

class UCapsuleComponent;

UCLASS()
class WIZARDSANDDRAGONS_API AFireStorm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireStorm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
		float DamageAmmount = 5;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
		float DamageInterval = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Duration)
		float Duration = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Duration)
		float InitialDelay = 2;

	bool bFireStormActive = false;

	FTimerHandle FireStormActivationTimer;

	UPROPERTY(EditDefaultsOnly, Category = Collider)
		UCapsuleComponent* CapsuleComp;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void DealDamage();

	void ActivateFireStorm();
	TArray<AActor*> AffectedActors;

	FTimerHandle DealDamageTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
