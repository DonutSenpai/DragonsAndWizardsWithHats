#pragma once

#include "SpellBase.h"
#include "MageOrbSpell.generated.h"


UCLASS()
class UMageOrbSpell : public USpellBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float TravelTime = 1.0f;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastSpellEffect(float TimeToTravel, FVector TargetLocation = FVector::ZeroVector);

protected:

	virtual void InternalCastSpell(FVector TargetLocation) override;

private:

	FTimerHandle HitExplosionHandle;

	UFUNCTION()
	void HitExplosion(FVector TargetLocation);

};