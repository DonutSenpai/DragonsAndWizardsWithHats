#pragma once

#include "SpellBase.h"
#include "FireboltSpell.generated.h"

//You should only ever be able to have one firebolt cast at a time, 
//meaning that the cooldown needs to be longer than the time it takes to hit the opponent
UCLASS()
class UFireboltSpell : public USpellBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float TravelTime = 0.75f;

	UPROPERTY()
	AActor* SpellTarget = nullptr;

	virtual void Server_CastSpell_Implementation(FVector TargetLocation) override;

};