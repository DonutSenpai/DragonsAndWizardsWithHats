#include "WADHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWADHealthComponent::UWADHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Get the owner of the component
	AActor* Owner = GetOwner();
}

// Called when the game starts
void UWADHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

bool UWADHealthComponent::GetInvincible() const
{
	return bInvincible;
}

int UWADHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

int UWADHealthComponent::GetMaxHealth() const
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

void UWADHealthComponent::DecreaseHealth(float Value)
{
	if (!bInvincible)
	{
		bInvincible = true;
		CurrentHealth -= Value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
		OnRep_CurrentHealth();
		GetWorld()->GetTimerManager().SetTimer(InvincibleTimer, this, &UWADHealthComponent::ToggleInvincibilityOff, 1.0f, false);	
	}
}

void UWADHealthComponent::IncreaseHealth(float Value)
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth += Value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
		OnRep_CurrentHealth();
	}
}

void UWADHealthComponent::ToggleInvincibilityOff()
{
	bInvincible = false;
}

void UWADHealthComponent::OnRep_CurrentHealth()
{
	if (IsDead())
	{
		OnDeath.Broadcast();
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
