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

	HealthComponent = CreateDefaultSubobject<UWADHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
}

void AAIDragon::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AAIDragon::OnDie);
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

void AAIDragon::Fire(const FVector& StartLocation, const FVector& ForwardDirection, AActor* DamageCauser, AController* EventInstigator)
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

		const FVector SpawnLocation = StartLocation + ForwardDirection * 200.0f;
		const FRotator SpawnRotation = ForwardDirection.Rotation();
		ADragonProjectile* NewProjectile = GetWorld()->SpawnActor<ADragonProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}

	FHitResult Hit;
	//WeaponSystem->OnFire.Broadcast(WeaponSystem, this, Hit);
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

	//PlayAnimMontage(GetRandomDeathAnimation());

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

class UAnimMontage* AAIDragon::GetRandomDeathAnimation() const
{
	return DeathAnims[FMath::RandRange(0, DeathAnims.Num() - 1)];
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
	if (!bIsAttacking)
	{
		bIsAttacking = true;

		PlayAnimMontage(GetRandomMeleeAnimation());
	}
}

void AAIDragon::RangedAttack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;

		PlayAnimMontage(RangedAnim);

		FHitResult Hit;
		FVector StartLocation;
		FRotator Forward;

		GetActorEyesViewPoint(StartLocation, Forward);

		Fire(StartLocation, Forward.Vector(), this, GetController());
	}	
}

float AAIDragon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OutDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority())
	{
		if (!HealthComponent->IsDead())
		{
			HealthComponent->DecreaseHealth(DamageAmount, DamageCauser);
			HealthComponent->OnRep_CurrentHealth();

			//PlayAnimMontage(GetRandomHitAnimation());
			BP_OnDamageTaken();
		}
	}
	else
	{
		//PlayAnimMontage(GetRandomHitAnimation());
		BP_OnDamageTaken();
	}

	return OutDamage;
}

float AAIDragon::InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float OutDamage = Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority())
	{
		if (!HealthComponent->IsDead())
		{
			HealthComponent->DecreaseHealth(OutDamage, DamageCauser);
			HealthComponent->OnRep_CurrentHealth();

			//PlayAnimMontage(GetRandomHitAnimation());
			BP_OnDamageTaken();
		}
	}
	else
	{
		//PlayAnimMontage(GetRandomHitAnimation());
		BP_OnDamageTaken();
	}

	return OutDamage;
}

float AAIDragon::InternalTakeRadialDamage(float Damage, FRadialDamageEvent const& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OutDamage = Super::InternalTakeRadialDamage(Damage, RadialDamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority())
	{
		if (!HealthComponent->IsDead())
		{
			HealthComponent->DecreaseHealth(OutDamage, DamageCauser);
			HealthComponent->OnRep_CurrentHealth();

			//PlayAnimMontage(GetRandomHitAnimation());
			BP_OnDamageTaken();
		}
	}
	else
	{
		//PlayAnimMontage(GetRandomHitAnimation());
		BP_OnDamageTaken();
	}
	return OutDamage;
}
