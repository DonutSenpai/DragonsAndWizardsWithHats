#pragma once

#include "SpellBase.h"
#include "ResurrectSpell.generated.h"

UCLASS()
class UResurrectSpell : public USpellBase
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnCastSpellEffect(float TimeToTravel, AActor* TargetActor = nullptr);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float TravelTime = 2.0f;

protected:

	virtual void InternalCastSpell(FVector TargetLocation) override;

	virtual void InternalDealDamage(const TArray<AActor*>& SpellTargets) override;

	UFUNCTION(NetMulticast, Reliable)
	void OnCastSpellEffectInternal(float TimeToTravel, AActor* TargetActor = nullptr);

	UPROPERTY(BlueprintReadOnly)
	AActor* SpellTarget = nullptr;

private:

	void Resurrect();

	FTimerHandle ResurrectHandle;

};