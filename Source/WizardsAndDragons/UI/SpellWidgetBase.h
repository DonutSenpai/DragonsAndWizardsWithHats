#pragma once

#include "Blueprint/UserWidget.h"
#include "SpellWidgetBase.generated.h"


UCLASS()
class USpellWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	class USpellBase* SpellComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	class AWADPlayerController* OwningController = nullptr;
};