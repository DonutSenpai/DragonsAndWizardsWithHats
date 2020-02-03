#pragma once

#include "GameFramework/Actor.h"
#include "MageOrb.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AMageOrb : public AActor
{
	GENERATED_BODY()

	AMageOrb();

public:

	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent)
	void OnMageOrbOverlapEffect();


	UFUNCTION()
	void OnMageOrbOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class USphereComponent* MageOrbCollision = nullptr;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	class UMageOrbSpell* OwningMageOrbSpellComponent = nullptr;


};