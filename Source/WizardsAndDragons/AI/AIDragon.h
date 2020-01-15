#pragma once

#include "GameFramework/Character.h"
#include "AIDragon.generated.h"

UCLASS()
class AAIDragon : public ACharacter
{
	GENERATED_BODY()
public:
	AAIDragon();

	virtual void BeginPlay() override;

	virtual bool ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	void OnDie();

	class AAIController* GetAIController() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Damage Taken"))
	void BP_OnDamageTaken();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Dead"))
	void BP_OnDead();

	UFUNCTION(BlueprintCallable)
	class UAnimMontage* GetRandomHitAnimation() const;

	UFUNCTION(BlueprintCallable)
	class UAnimMontage* GetRandomDeathAnimation() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetHealth() const { return HealthCurrent; };

protected:
	UFUNCTION()
	void OnRep_CurrentHealth();

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<class UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<class UAnimMontage*> DeathAnims;

	UPROPERTY(EditAnywhere, Category = Health)
	float Health = 100.0f;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float HealthCurrent = 100.0f;

	bool bDead = false;

	UFUNCTION()
	void DoRagdoll();

	virtual float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};

