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

	UPROPERTY(BlueprintReadOnly)
	AActor* SpellTarget = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastSpellEffect(AActor* TargetActor, float TimeToTravel);

	UFUNCTION()
	virtual void Server_CastSpell_Implementation(FVector TargetLocation) override;

	UFUNCTION()
	virtual void Server_DealDamage_Implementation(FVector TargetLocation) override;

private:

	void DealDamage();

	FTimerHandle DealDamageHandle;

};