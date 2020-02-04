#include "MageOrb.h"
#include <Components/SphereComponent.h>
#include <Components/SceneComponent.h>
#include "../Components/WADHealthComponent.h"
#include "MageOrbSpell.h"

AMageOrb::AMageOrb()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComponent);

	MageOrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Mage Orb Collision"));
	MageOrbCollision->AttachTo(RootComponent);
}

void AMageOrb::BeginPlay()
{
	Super::BeginPlay();

	if (OwningMageOrbSpellComponent->GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		MageOrbCollision->OnComponentBeginOverlap.AddDynamic(this, &AMageOrb::OnMageOrbOverlap);
		MageOrbCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}

}

void AMageOrb::OnMageOrbOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OwningMageOrbSpellComponent || OtherActor == OwningMageOrbSpellComponent->GetOwner()) return;

	if (UWADHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UWADHealthComponent>())
	{		
		UE_LOG(LogTemp, Warning, TEXT("Damage is Dealt"));
		HealthComponent->DecreaseHealth(OwningMageOrbSpellComponent->Damage, OwningMageOrbSpellComponent->GetOwner());
		OnMageOrbOverlapEffect(OtherActor);
	}


}
