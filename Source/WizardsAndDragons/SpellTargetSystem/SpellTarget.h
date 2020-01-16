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


protected:

	void BeginPlay() override;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SpellCollision;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UDecalComponent* TargetDecal = nullptr;
};