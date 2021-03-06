#pragma once

#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "AIDragon.generated.h"

class ADragonProjectile;
class AFireStorm;
class UWADHealthComponent;
class UAnimMontage;
class AAIController;

UCLASS()
class AAIDragon : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AAIDragon();

	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(Team)); };

	UFUNCTION(BlueprintPure, Category = AttackBehaviour)
		int32 GetTeam() const { return Team; }

	UFUNCTION(BlueprintPure, Category = AttackBehaviour)
		bool GetProjectileAttackReady() const { return bProjectileAttackReady && bRangedAttackReady; }

	UFUNCTION(BlueprintPure, Category = AttackBehaviour)
		bool GetFireStormAttackReady() const { return bFireStormAttackReady && bRangedAttackReady; }

	UFUNCTION(BlueprintPure, Category = AttackBehaviour)
		bool GetAnyRangedAttackReady() const { return bFireStormAttackReady || bProjectileAttackReady && bRangedAttackReady; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team)
		int32 Team = 0;

	UPROPERTY(EditDefaultsOnly, Category = AttackBehaviour)
		TSubclassOf<ADragonProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = AttackBehaviour)
		TSubclassOf<AFireStorm> FireStormClass;

	 AAIController* GetAIController() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Damage Taken"))
		void BP_OnDamageTaken();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Dead"))
		void BP_OnDead();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Reset Melee Attack"))
		void BP_OnResetMelee();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackBehaviour)
		float MeleeAttackCooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackBehaviour)
		bool bRangedAttackReady = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackBehaviour)
		float RangedAttackCooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackBehaviour)
		bool bProjectileAttackReady = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackBehaviour)
		float ProjectileAttackCooldown = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackBehaviour)
		bool bFireStormAttackReady = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackBehaviour)
		float FireStormAttackCooldown = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Scoring)
		int ScoreValue = 100;

	bool bDead = false;

	virtual void BeginPlay() override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	void Fire(const FVector& StartLocation, const FRotator& ForwardRotation, AActor* DamageCauser, AController* EventInstigator);

	UFUNCTION()
		void DoRagdoll();

	UFUNCTION()
		void OnDie();

	UFUNCTION()
		void OnHit(float HealthChange, AActor* InstigatingActor);

	void Destroyed();

	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetRandomHitAnimation() const;

	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetRandomMeleeAnimation() const;

	UFUNCTION(BlueprintCallable)
		void MeleeAttack();

	UFUNCTION(BlueprintCallable)
		void BiteAttack();

	UFUNCTION(BlueprintCallable)
		void ProjectileAttack();

	UFUNCTION(BlueprintCallable)
		void FireStormAttack(FVector SpawnLocation);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_MeleeAttack();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_BiteAttack();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_ProjectileAttack();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_FireStormAttack(FVector SpawnLocation);


	void SpawnFireStorm(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* DamageCauser, AController* EventInstigator);

	UFUNCTION(BlueprintCallable, Category = AttackBehaviour)
		void ResetMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = AttackBehaviour)
		void ResetRangedAttack();

	UFUNCTION(BlueprintCallable, Category = AttackBehaviour)
		void ResetProjectileAttack();

	UFUNCTION(BlueprintCallable, Category = AttackBehaviour)
		void ResetFireStormAttack();
	
	FTimerHandle MeleeAttackCooldownTimer;

	FTimerHandle RangedAttackCooldownTimer;

	FTimerHandle ProjectileAttackCooldownTimer;

	FTimerHandle FireStormAttackCooldownTimer;
};

