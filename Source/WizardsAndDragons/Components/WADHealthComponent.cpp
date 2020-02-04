#include "WADHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UWADHealthComponent::UWADHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bEditableWhenInherited = true;
}

// Called when the game starts
void UWADHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// Get the owner of the component
	Owner = GetOwner();
}

bool UWADHealthComponent::GetInvincible() const
{
	return bInvincible;
}

float UWADHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UWADHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UWADHealthComponent::ModifyMaxHealth(float Value)
{
	MaxHealth = Value;
}

void UWADHealthComponent::ModifyHealth(float Value)
{
	CurrentHealth = Value;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	OnRep_CurrentHealth();
}

void UWADHealthComponent::DecreaseHealth(float Value, AActor* InstigatingActor)
{
	if (!bInvincible && InstigatingActor != Owner)
	{
		bInvincible = true;

		CurrentHealth -= Value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

		OnRep_CurrentHealth();

		OnHealthChanged.Broadcast(Value, InstigatingActor);

		GetWorld()->GetTimerManager().SetTimer(InvincibleTimer, this, &UWADHealthComponent::ToggleInvincibilityOff, 1.0f, false);
	}
}

void UWADHealthComponent::IncreaseHealth(float Value, AActor* InstigatingActor)
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth += Value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

		OnRep_CurrentHealth();

		OnHealthChanged.Broadcast(Value, InstigatingActor);
	}
}

void UWADHealthComponent::ToggleInvincibilityOff()
{
	bInvincible = false;
}

void UWADHealthComponent::OnRep_CurrentHealth()
{
	if (FMath::IsWithinInclusive(CurrentHealth, (MaxHealth * 3 / 4) + 1, MaxHealth))
	{
		HealthStateEnum = EHealthStateEnum::HE_FullHealth;
	}
	else if (FMath::IsWithinInclusive(CurrentHealth, (MaxHealth/2) + 1, MaxHealth * 3 / 4))
	{
		HealthStateEnum = EHealthStateEnum::HE_ThreeQuarterHealth;
	}
	else if (FMath::IsWithinInclusive(CurrentHealth, (MaxHealth * 1 / 4) + 1, MaxHealth / 2))
	{
		HealthStateEnum = EHealthStateEnum::HE_HalfHealth;
	}
	else if (FMath::IsWithinInclusive(CurrentHealth, 1.0f, MaxHealth * 1 / 4))
	{
		HealthStateEnum = EHealthStateEnum::HE_QuarterHealth;
	}

	if (IsDead())
	{
		OnDeath.Broadcast();
		HealthStateEnum = EHealthStateEnum::HE_NoHealth;
	}
}

bool UWADHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

void UWADHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWADHealthComponent, CurrentHealth);
}

