#include "AIDragon.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "../Components/WADHealthComponent.h"
#include "DragonProjectile.h"

AAIDragon::AAIDragon()
{
	SetReplicates(true);

	HealthComp = CreateDefaultSubobject<UWADHealthComponent>(TEXT("HealthComp"));
	HealthComp->SetIsReplicated(true);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(120.0f, 0.0f, -40.0f));
}

void AAIDragon::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp)
	{
		HealthComp->OnDeath.AddDynamic(this, &AAIDragon::OnDie);
		HealthComp->OnHealthChanged.AddDynamic(this, &AAIDragon::OnHit);
	}
}

bool AAIDragon::ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	return true;
}

void AAIDragon::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void AAIDragon::Fire(const FVector& StartLocation, const FRotator& ForwardRotation, AActor* DamageCauser, AController* EventInstigator)
{
	if (ProjectileClass.Get() == nullptr)
	{
		return;
	}

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const FVector SpawnLocation = StartLocation;

		const FRotator SpawnRotation = ForwardRotation;
		ADragonProjectile* NewProjectile = GetWorld()->SpawnActor<ADragonProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AAIDragon::OnDie()
{
	bDead = true;
	BP_OnDead();

	if (AAIController* AIController = GetAIController())
	{
		UBrainComponent* Brain = AIController->BrainComponent;
		if (Brain != nullptr)
		{
			Brain->StopLogic(FString(TEXT("Death")));
		}
	}

	PlayAnimMontage(DeathAnim);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAIDragon::DoRagdoll, FMath::RandRange(0.25f, 0.5f), false);
}

AAIController* AAIDragon::GetAIController() const
{
	return CastChecked<AAIController>(Controller, ECastCheckedType::NullAllowed);
}

class UAnimMontage* AAIDragon::GetRandomHitAnimation() const
{
	return HitReactions[FMath::RandRange(0, HitReactions.Num() - 1)];
}

class UAnimMontage* AAIDragon::GetRandomMeleeAnimation() const
{
	return MeleeAnims[FMath::RandRange(0, MeleeAnims.Num() - 1)];
}

void AAIDragon::DoRagdoll()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("NoCollision")));
	GetMesh()->SetCollisionProfileName(FName(TEXT("PhysicsActor")));
	GetMesh()->SetSimulatePhysics(true);
	SetLifeSpan(10.0f);
}

void AAIDragon::MeleeAttack()
{
	if (bMeleeAttackReady)
	{
		bMeleeAttackReady = false;

		PlayAnimMontage(GetRandomMeleeAnimation());

		GetWorld()->GetTimerManager().SetTimer(MeleeAttackCooldownTimer, this, &AAIDragon::ResetMeleeAttack, MeleeAttackCooldown, false);
	}
}

void AAIDragon::BiteAttack()
{
	if (bMeleeAttackReady)
	{
		bMeleeAttackReady = false;

		PlayAnimMontage(BiteAnim);

		GetWorld()->GetTimerManager().SetTimer(MeleeAttackCooldownTimer, this, &AAIDragon::ResetMeleeAttack, MeleeAttackCooldown, false);
	}
}

void AAIDragon::ResetMeleeAttack()
{
	bMeleeAttackReady = true;

	BP_OnResetMelee();
}

void AAIDragon::ResetProjectileAttack()
{
	bProjectileAttackReady = true;
}

void AAIDragon::ProjectileAttack()
{
	if (bProjectileAttackReady)
	{
		bProjectileAttackReady = false;

		PlayAnimMontage(ProjectileAnim);

		FVector StartLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ForwardRotation = ProjectileSpawnPoint->GetComponentRotation();

		Fire(StartLocation, ForwardRotation, this, GetController());

		GetWorld()->GetTimerManager().SetTimer(ProjectileAttackCooldownTimer, this, &AAIDragon::ResetProjectileAttack, ProjectileAttackCooldown, false);
	}	
}


void AAIDragon::OnHit(float HealthChange, AActor* InstigatingActor)
{
	if (HasAuthority())
	{
		if (!HealthComp->IsDead())
		{
			/*HealthComponent->DecreaseHealth(DamageAmount, DamageCauser);
			HealthComponent->OnRep_CurrentHealth();*/

			PlayAnimMontage(GetRandomHitAnimation());
			BP_OnDamageTaken();
		}
	}
	else
	{
		PlayAnimMontage(GetRandomHitAnimation());
		BP_OnDamageTaken();
	}
}