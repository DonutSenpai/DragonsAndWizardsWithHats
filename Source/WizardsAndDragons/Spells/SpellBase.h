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

	//The replicated VFX and/or Sound effect when the spell is cast
	//@TargetLocation the location of impact(Where the spell should hit)
	UFUNCTION(BlueprintImplementableEvent)
	void OnCastSpellEffects(FVector TargetLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnCastSpellEffects(FVector TargetLocation);

	//Always call the Super function, it handles the cooldown automatically
	//Should return wether it successfully casts the spell or not
	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void Server_CastSpell(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsOnCooldown() { return CurrentCooldown > 0.0f; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetCurrentCooldown() { return CurrentCooldown; }

protected:
	UPROPERTY(Replicated)
	float CurrentCooldown = 0.0f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
 

	FTimerHandle CooldownTimerHandle;
	void CountDownCooldown();


	class AWizardsAndDragonsCharacter* OwningCharacter = nullptr;

};