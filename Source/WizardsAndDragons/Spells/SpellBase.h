#pragma once

#include "Components/ActorComponent.h"
#include "SpellBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class USpellBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = SpellProperties)
	FName Name;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = SpellProperties)
	float Range = 50.f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = SpellProperties)

	float Radius = 5.f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = SpellProperties)
	float Cooldown = 1.0f;


	//Always call the Super function, it handles the cooldown automatically
	UFUNCTION(Server, Reliable)
	virtual void Server_CastSpell(FVector TargetLocation);

	FORCEINLINE bool GetIsOnCooldown() { return CurrentCooldown <= 0.0f; }

protected:
	UPROPERTY(Replicated)
	float CurrentCooldown = 0.0f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;


	FTimerHandle CooldownTimerHandle;
	void CountDownCooldown();


	class AWizardsAndDragonsCharacter* OwningCharacter = nullptr;

};