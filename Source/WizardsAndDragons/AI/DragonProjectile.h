// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonProjectile.generated.h"

class USphereComponent;

UCLASS()
class WIZARDSANDDRAGONS_API ADragonProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADragonProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
		USphereComponent* SphereComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Explode();

	UFUNCTION(BlueprintImplementableEvent, Category = DragonProjectile, meta = (DisplayName = "On Explode"))
		void BP_OnExplode();

	UPROPERTY(EditAnywhere, Category = Movement)
		float ProjectileVelocity = 3500.0f;

};
