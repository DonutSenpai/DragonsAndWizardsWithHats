#include "WADHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

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
		OnHealthChanged.Broadcast(Value, InstigatingActor);
		OnRep_CurrentHealth();
		GetWorld()->GetTimerManager().SetTimer(InvincibleTimer, this, &UWADHealthComponent::ToggleInvincibilityOff, 1.0f, false);

		UE_LOG(LogTemp, Warning, TEXT("Health was decreased by: %f"), Value);
	}
}

void UWADHealthComponent::IncreaseHealth(float Value, AActor* InstigatingActor)
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth += Value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
		OnHealthChanged.Broadcast(Value, InstigatingActor);
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

