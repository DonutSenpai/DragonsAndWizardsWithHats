#include "SpellTargetSystemComponent.h"
#include "Components/DecalComponent.h"

USpellTargetSystemComponent::USpellTargetSystemComponent()
{
	SpellTarget = CreateDefaultSubobject<UDecalComponent>(TEXT("SpellTarget"));
}
