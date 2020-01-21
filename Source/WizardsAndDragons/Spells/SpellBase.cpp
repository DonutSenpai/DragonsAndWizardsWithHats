#include "SpellBase.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Components/WADHealthComponent.h"


void USpellBase::Server_CastSpell_Implementation(FVector TargetLocation)
{
	if (CurrentCooldown > 0.0f) return;

	CurrentCooldown = Cooldown;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USpellBase::CountDownCooldown, 0.1f, true, 0.0f);
	Multicast_OnCastSpellEffects(TargetLocation);	

}

void USpellBase::Multicast_OnCastSpellEffects_Implementation(FVector TargetLocation)
{
	OnCastSpellEffects(TargetLocation);
}

void USpellBase::Server_DealDamage_Implementation(FVector TargetLocation)
{
	TArray<AActor*> OverlappedActors = GetSpellTargetsInRadius(TargetLocation);	
	
	for (AActor* OverlappedActor : OverlappedActors)
	{
		if (UWADHealthComponent* HealthComponent = OverlappedActor->FindComponentByClass<UWADHealthComponent>())
		{
			HealthComponent->DecreaseHealth(Damage);
		}
	}
}

TArray<AActor*> USpellBase::GetSpellTargetsInRadius(FVector TargetLocation)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TargetLocation, Radius, ObjectTypes, nullptr, IgnoredActors, OverlappedActors);

	return OverlappedActors;
}

void USpellBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(USpellBase, CurrentCooldown, COND_OwnerOnly);
}

void USpellBase::BeginPlay()
{
	Super::BeginPlay();


}

void USpellBase::CountDownCooldown()
{
	CurrentCooldown -= 0.1f;

	if (CurrentCooldown <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}
