#pragma once

#include "Components/ActorComponent.h"
#include "SpellBase.generated.h"

class USlateBrushAsset;

//This is a base class that contains absolute bare minimum functionality.
UCLASS(Blueprintable, BlueprintType)
class USpellBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	FName Name = "None";

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	FSlateBrush Icon;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float Range = 50.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float Radius = 5.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float Cooldown = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SpellProperties)
	float Damage = 1.f;

	//Handles the cooldown automatically.
	UFUNCTION(Server, Reliable)
	void Server_CastSpell(FVector TargetLocation);

	//Separate event from cast spell because it might want to be timed to an effect etc. Calls the InternalDealDamage function,
	//which by default deals AOE damage. Can be overridden to have single target damage instead of AOE
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_DealDamage(const TArray<AActor*> &SpellTargets);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsOnCooldown() { return CurrentCooldown > 0.0f; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetCurrentCooldown() { return CurrentCooldown; }

protected:

	UFUNCTION()
	virtual void InternalCastSpell(FVector TargetLocation);

	//By default deals AOE damage. Can be overridden to have single target damage instead of AOE
	UFUNCTION()
	virtual void InternalDealDamage(const TArray<AActor*> &SpellTargets);


	//Gets possible spell target actors in radius. This function ASSUMES that
	//all PAWNS are damageable(aka have a health component)
	UFUNCTION(BlueprintCallable, Category = SpellHelperFunctions)
	TArray<AActor*> GetSpellTargetsInRadius(FVector TargetLocation);

	UPROPERTY(Replicated)
	float CurrentCooldown = 0.0f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
 

	FTimerHandle CooldownTimerHandle;
	void CountDownCooldown();


	class AWizardsAndDragonsCharacter* OwningCharacter = nullptr;

};