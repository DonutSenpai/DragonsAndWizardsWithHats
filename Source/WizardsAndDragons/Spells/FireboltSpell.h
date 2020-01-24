#pragma once

#include "SpellBase.h"
#include "FireboltSpell.generated.h"


//Firebolt: Target seeking, deals damage to the (one) target closest to the center of the spell.
//TIP: You should only ever be able to have one firebolt cast at a time, 
//meaning that the cooldown needs to be longer than the time it takes to hit the opponent.
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
	void OnCastSpellEffect(float TimeToTravel, FVector TargetLocation = FVector::ZeroVector, AActor* TargetActor = nullptr);

protected:
	
	virtual void InternalCastSpell(FVector TargetLocation) override;

	virtual void InternalDealDamage(const TArray<AActor*> &SpellTargets) override;

	UFUNCTION(NetMulticast, Reliable)
	void OnCastSpellEffectInternal(float TimeToTravel, FVector TargetLocation = FVector::ZeroVector, AActor* TargetActor = nullptr);


private:

	void DealDamage();

	FTimerHandle DealDamageHandle;

};