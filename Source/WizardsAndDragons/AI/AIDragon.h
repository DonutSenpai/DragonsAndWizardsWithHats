#pragma once

#include "GameFramework/Character.h"
#include "AIDragon.generated.h"

class ADragonProjectile;
class UWADHealthComponent;
class UAnimMontage;
class AAIController;

UCLASS()
class AAIDragon : public ACharacter
{
	GENERATED_BODY()
public:
	AAIDragon();

	virtual void BeginPlay() override;

	virtual bool ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION(BlueprintPure)
		int32 GetTeam() const { return Team; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team)
		int32 Team = 0;

	void Fire(const FVector& StartLocation, const FRotator& ForwardRotation, AActor* DamageCauser, AController* EventInstigator);

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<ADragonProjectile> ProjectileClass;

	UFUNCTION()
		void OnDie();

	UFUNCTION()
		void OnHit(float HealthChange, AActor* InstigatingActor);

	 AAIController* GetAIController() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Damage Taken"))
		void BP_OnDamageTaken();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Dead"))
		void BP_OnDead();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Reset Melee Attack"))
		void BP_OnResetMelee();

	UFUNCTION(BlueprintCallable)
		 UAnimMontage* GetRandomHitAnimation() const;

	UFUNCTION(BlueprintCallable)
		 UAnimMontage* GetRandomMeleeAnimation() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		 UWADHealthComponent* HealthComp;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<UAnimMontage*> HitReactions;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<UAnimMontage*> MeleeAnims;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* BiteAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
			 UAnimMontage*  ProjectileAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackBehaviour)
		bool bMeleeAttackReady = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackBehaviour)
		bool bProjectileAttackReady = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackBehaviour)
		float ProjectileAttackCooldown = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackBehaviour)
		float MeleeAttackCooldown = 1.5f;

	bool bDead = false;

	UFUNCTION()
		void DoRagdoll();

	UFUNCTION(BlueprintCallable)
		void MeleeAttack();

	UFUNCTION(BlueprintCallable)
		void BiteAttack();

	UFUNCTION(BlueprintCallable)
		void ProjectileAttack();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void ResetMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void ResetProjectileAttack();
	
	FTimerHandle MeleeAttackCooldownTimer;

	FTimerHandle ProjectileAttackCooldownTimer;
};

