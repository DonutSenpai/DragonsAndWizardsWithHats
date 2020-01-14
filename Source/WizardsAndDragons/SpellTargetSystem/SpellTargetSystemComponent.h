#pragma once
#include "Components/ActorComponent.h"
#include "SpellTargetSystemComponent.generated.h"

UCLASS(BlueprintType, Blueprintable)
class USpellTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpellTargetSystemComponent();

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* SpellTarget;
};