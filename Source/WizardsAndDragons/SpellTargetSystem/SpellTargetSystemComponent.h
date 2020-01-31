#pragma once
#include "Components/ActorComponent.h"
#include "SpellTargetSystemComponent.generated.h"


//TO DO: Limit targeting to the actual range of the spell.
UCLASS(BlueprintType, Blueprintable)
class USpellTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpellTargetSystemComponent();

	UPROPERTY(NoClear, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASpellTarget> SpellTargetClass;

	UPROPERTY(NoClear, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASpellTarget> SpellTargetRangeIndicatorClass;

	UFUNCTION(BlueprintCallable, Category = SpellTargetSystem)
	void StartSpellTargetSystem(class USpellBase* Spell);

	UFUNCTION(BlueprintCallable, Category = SpellTargetSystem)
	void StopSpellTargetSystem();

	//Set from the controller
	UPROPERTY()
	class AWADPlayerController* OwningController;
	
	UPROPERTY(BlueprintReadWrite)
	class ASpellTarget* SpellTarget = nullptr;

	UPROPERTY(BlueprintReadWrite)
	class ASpellTarget* SpellTargetRangeIndicator = nullptr;

	UFUNCTION(BlueprintCallable)
	void CastSpell();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsSystemActive(){ return bIsSystemActive; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsSystemAlreadyActive(USpellBase* IsSpellActive);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanActivateSystem(USpellBase* SpellToActivate);

protected:

	bool bIsSystemActive = false;

	UPROPERTY()
	class USpellBase* SelectedSpell = nullptr;

	FTimerHandle SpellTargetHandle;
	UFUNCTION()
	void SimulateSpellTarget();

	FHitResult OutHitMousePosition;

	//Checks stuff like if the target is in the range of the spell
	bool CanCastSpell();
};