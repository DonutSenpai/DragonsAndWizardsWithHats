#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WADHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedEvent, float, ChangeValue, AActor*, InstigatingActor);

UCLASS(Blueprintable)
class  UWADHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWADHealthComponent();

	// Get invincible status of the character
	UFUNCTION(BlueprintPure, Category = "Health")
		bool GetInvincible() const;

	// Get the current health of the character
	UFUNCTION(BlueprintPure, Category = "Health")
		int GetCurrentHealth() const;

	// Get the max health of the character
	UFUNCTION(BlueprintPure, Category = "Health")
		int GetMaxHealth() const;

	// Change Actor MaxHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ModifyMaxHealth(float Value);

	// Change CurrentHealth to value
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ModifyHealth(float Value);

	//  Remove value CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void DecreaseHealth(float Value, AActor* InstigatingActor);

	//  Blueprint event to run on decreased health
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
		void OnDecreaseHealth(float Value, AActor* InstigatingActor);

	// Add value to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void IncreaseHealth(float Value, AActor* InstigatingActor);

	// Blueprint event to run on increased health
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
		void OnIncreaseHealth(float Value, AActor* InstigatingActor);

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	// Fires when health reaches zero
	UPROPERTY(BlueprintAssignable, Category = "Health")
		FDeathEvent OnDeath;

	// Fires when health reaches zero
	UPROPERTY(BlueprintAssignable, Category = "Health")
		FHealthChangedEvent OnHealthChanged;

	UFUNCTION()
		void OnRep_CurrentHealth();

protected:
	// The current value of heatlh
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, Category = "Health")
		float  CurrentHealth;

	// The max health of the component
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100;

	// Invincible flag - Invincibles takes damage but doesnt lose health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool bInvincible = false;

	// Add value to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ToggleInvincibilityOff();

private:
	FTimerHandle InvincibleTimer;

	// Called when the game starts
	virtual void BeginPlay() override;

};
