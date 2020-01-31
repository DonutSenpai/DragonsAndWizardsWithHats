
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

	UPROPERTY(BlueprintReadOnly)
	class AWizardsAndDragonsCharacter* OwnedCharacter;

	void SetRotateCameraTrue();
	void SetRotateCameraFalse();

	float PreviousMouseX = 0.0f;
	float PreviousMouseY = 0.0f;

	UFUNCTION(BlueprintCallable)
	void StartSpellTargetSystem(class USpellBase* Spell);

	void OnSpellCast();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpellTargetSystemComponent* SpellTargetSystem = nullptr;

private:

	void PrintLeftClick();
	void CastSpell();


};