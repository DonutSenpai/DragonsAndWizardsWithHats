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

	UFUNCTION()
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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
		class UWADHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<class UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<class UAnimMontage*> DeathAnims;

	bool bDead = false;

	UFUNCTION()
		void DoRagdoll();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual float InternalTakeRadialDamage(float Damage, FRadialDamageEvent const& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

