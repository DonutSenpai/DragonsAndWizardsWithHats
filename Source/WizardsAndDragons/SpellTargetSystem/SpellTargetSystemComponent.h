#pragma once
#include "Components/ActorComponent.h"
#include "SpellTargetSystemComponent.generated.h"

UCLASS(BlueprintType, Blueprintable)
class USpellTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpellTargetSystemComponent();

	UPROPERTY(NoClear, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASpellTarget> SpellTargetClass;

	UFUNCTION(BlueprintCallable, Category = SpellTargetSystem)
	void StartSpellTargetSystem(float Radius);

	UFUNCTION(BlueprintCallable, Category = SpellTargetSystem)
	void StopSpellTargetSystem();

	//Set from the controller
	UPROPERTY()
	class AWADPlayerController* OwningController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ASpellTarget* SpellTarget = nullptr;

protected:



	FTimerHandle SpellTargetHandle;
	UFUNCTION()
	void SimulateSpellTarget();
};