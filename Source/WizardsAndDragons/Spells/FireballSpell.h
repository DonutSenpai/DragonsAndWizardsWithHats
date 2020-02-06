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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float ExplosionLaunchPower = 100.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float ExplosionLaunchHeightAdd = -10.f;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastSpellEffect(float TimeToTravel, FVector TargetLocation = FVector::ZeroVector);

	UFUNCTION(BlueprintImplementableEvent)
	void OnExplodeHitTargets(const TArray<AActor*> &DamagedTargets);

protected:

	virtual void InternalCastSpell(FVector TargetLocation) override;

	UFUNCTION(NetMulticast, Reliable)
	void OnCastSpellEffectInternal(float TimeToTravel, FVector TargetLocation = FVector::ZeroVector);

private:

	FTimerHandle HitExplosionHandle;

	UFUNCTION()
	void HitExplosion(FVector TargetLocation);

	FVector SpellTargetLocation = FVector::ZeroVector;



};