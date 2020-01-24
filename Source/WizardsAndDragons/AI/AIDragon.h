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

	UFUNCTION(BlueprintCallable)
		class UAnimMontage* GetRandomMeleeAnimation() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
		class UWADHealthComponent* HealthComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<class UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<class UAnimMontage*> DeathAnims;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<class UAnimMontage*> MeleeAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsAttacking = false;

	bool bDead = false;

	UFUNCTION()
		void DoRagdoll();

	UFUNCTION(BlueprintCallable)
		void MeleeAttack();

	UFUNCTION(BlueprintCallable)
		void RangedAttack();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual float InternalTakeRadialDamage(float Damage, FRadialDamageEvent const& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

