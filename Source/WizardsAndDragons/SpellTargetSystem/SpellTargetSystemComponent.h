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
	void StartSpellTargetSystem(class USpellBase* Spell);

	UFUNCTION(BlueprintCallable, Category = SpellTargetSystem)
	void StopSpellTargetSystem();

	//Set from the controller
	UPROPERTY()
	class AWADPlayerController* OwningController;
	
	UPROPERTY(BlueprintReadWrite)
	class ASpellTarget* SpellTarget = nullptr;

	UFUNCTION(BlueprintCallable)
	void CastSpell();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsSystemActive(){ return bIsSystemActive; }


protected:

	bool bIsSystemActive = false;

	UPROPERTY()
	class USpellBase* SelectedSpell = nullptr;

	FTimerHandle SpellTargetHandle;
	UFUNCTION()
	void SimulateSpellTarget();

	FHitResult OutHitMousePosition;
};