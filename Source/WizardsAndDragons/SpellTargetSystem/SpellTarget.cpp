#include "SpellTarget.h"
#include <Components/DecalComponent.h>
#include <Components/SphereComponent.h>

ASpellTarget::ASpellTarget()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TargetDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetDecal"));
	TargetDecal->AttachTo(RootComponent);
	SpellCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpellCollision"));
	SpellCollision->AttachTo(RootComponent);
}

void ASpellTarget::SetRadius(float Radius)
{
	TargetDecal->DecalSize = FVector(Radius);
	SpellCollision->SetSphereRadius(Radius);
	
}
