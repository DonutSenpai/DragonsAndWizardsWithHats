#include "SpellBase.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Components/WADHealthComponent.h"


void USpellBase::Server_CastSpell_Implementation(FVector TargetLocation)
{
	if (CurrentCooldown > 0.0f) return;

	CurrentCooldown = Cooldown;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USpellBase::CountDownCooldown, 0.1f, true, 0.0f);

	InternalCastSpell(TargetLocation);
}
void USpellBase::Server_DealDamage_Implementation(const TArray<AActor*>& SpellTargets)
{
	InternalDealDamage(SpellTargets);
}

void USpellBase::InternalCastSpell(FVector TargetLocation)
{

}

void USpellBase::InternalDealDamage(const TArray<AActor*>& SpellTargets)
{
	for (AActor* SpellTarget : SpellTargets)
	{
		if (UWADHealthComponent* HealthComponent = SpellTarget->FindComponentByClass<UWADHealthComponent>())
		{
			HealthComponent->DecreaseHealth(Damage, GetOwner());
		}
	}
}

TArray<AActor*> USpellBase::GetSpellTargetsInRadius(FVector TargetLocation, bool bIgnoreCaster)
{
	TArray<AActor*> IgnoredActors;

	if (bIgnoreCaster)
	{
		IgnoredActors.Add(GetOwner());
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> OverlappedActors;
	bool bDidOverlapAnyActors = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TargetLocation, Radius, ObjectTypes, nullptr, IgnoredActors, OverlappedActors);
	UE_LOG(LogTemp, Warning, TEXT("Did overlap any actors: %d"), bDidOverlapAnyActors);
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
