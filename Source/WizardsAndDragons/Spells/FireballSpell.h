#pragma once

#include "SpellBase.h"
#include "FireballSpell.generated.h"


//Deals explosion damage in radius on hit
UCLASS()
class UFireballSpell : public USpellBase
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

	FVector SpellTargetLocation = FVector::ZeroVector;


};