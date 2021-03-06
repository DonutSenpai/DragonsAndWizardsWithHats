#pragma once

#include "GameFramework/Actor.h"
#include "SpellTarget.generated.h"

UCLASS()
class ASpellTarget : public AActor
{
	GENERATED_BODY()

	ASpellTarget();


public:

	UFUNCTION()
	void SetRadius(float Radius);

	UFUNCTION()
	void SetRange(float Range);

	UFUNCTION(BlueprintCallable)
	void SetIsInRange(bool IsInRange);


protected:

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* InRangeDecalMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* OutOfRangeDecalMaterial = nullptr;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SpellCollision;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UDecalComponent* TargetDecal = nullptr;
};