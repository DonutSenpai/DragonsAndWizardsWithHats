
#pragma once

#include "GameFramework/PlayerController.h"
#include "WADPlayerController.generated.h"

UCLASS()
class AWADPlayerController : public APlayerController
{
	GENERATED_BODY()

	AWADPlayerController();
public:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	class AWizardsAndDragonsCharacter* OwnedCharacter;

	void SetRotateCameraTrue();
	void SetRotateCameraFalse();

	float PreviousMouseX = 0.0f;
	float PreviousMouseY = 0.0f;

	UFUNCTION(BlueprintCallable)
	void StartSpellTargetSystem(class USpellBase* Spell);

	void OnSpellCast();

protected:

	//----On Death/Resurrect------

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION()
	void Internal_OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void OnResurrect();

	UFUNCTION()
	void Internal_OnResurrect();


	//------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpellTargetSystemComponent* SpellTargetSystem = nullptr;

private:

	void PrintLeftClick();
	void CastSpell();


};