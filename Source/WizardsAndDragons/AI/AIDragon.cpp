#include "AIDragon.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "../Components/WADHealthComponent.h"

AAIDragon::AAIDragon()
{
	HealthComponent = CreateDefaultSubobject<UWADHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);

	SetReplicates(true);
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

void AAIDragon::DoRagdoll()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("NoCollision")));
	GetMesh()->SetCollisionProfileName(FName(TEXT("PhysicsActor")));
	GetMesh()->SetSimulatePhysics(true);
	SetLifeSpan(10.0f);
}

float AAIDragon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OutDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority())
	{
		if (!HealthComponent->IsDead())
		{
			HealthComponent->DecreaseHealth(DamageAmount);
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
			HealthComponent->DecreaseHealth(OutDamage);
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
			HealthComponent->DecreaseHealth(OutDamage);
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
