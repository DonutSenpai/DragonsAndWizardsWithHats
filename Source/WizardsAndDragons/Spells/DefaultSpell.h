#pragma once

#include "SpellBase.h"
#include "DefaultSpell.generated.h"


//This class contains more functionality than SpellBase and is intended as a base template for spells made entirely in blueprints.
UCLASS()
class UDefaultSpell : public USpellBase
{
	GENERATED_BODY()

public:
	//The replicated VFX and/or Sound effect when the spell is cast
//@TargetLocation the location of impact(Where the spell should hit)
	UFUNCTION(BlueprintImplementableEvent)
		void OnCastSpellEffects(FVector TargetLocation);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_OnCastSpellEffects(FVector TargetLocation);
};