#pragma once

#include "Components/ActorComponent.h"
#include "SpellBase.generated.h"

class USlateBrushAsset;

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

	//The replicated VFX and/or Sound effect when the spell is cast
	//@TargetLocation the location of impact(Where the spell should hit)
	UFUNCTION(BlueprintImplementableEvent)
	void OnCastSpellEffects(FVector TargetLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnCastSpellEffects(FVector TargetLocation);

	//Always call the Super function, it handles the cooldown automatically
	UFUNCTION(Server, Reliable)
	virtual void Server_CastSpell(FVector TargetLocation);

	//Deals damage instantly in radius AOE when called. Separate event from cast spell because it might want to be timed to an effect etc
	//Can be overridden to have single target damage instead of AOE
	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void Server_DealDamage(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsOnCooldown() { return CurrentCooldown > 0.0f; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetCurrentCooldown() { return CurrentCooldown; }

protected:

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