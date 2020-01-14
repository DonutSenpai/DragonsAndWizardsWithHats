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

	class AWizardsAndDragonsCharacter* OwnedCharacter;

	void SetRotateCameraTrue();
	void SetRotateCameraFalse();

	float PreviousMouseX = 0.0f;
	float PreviousMouseY = 0.0f;
private:

	void PrintLeftClick();
	void PrintLeftRelease();


};