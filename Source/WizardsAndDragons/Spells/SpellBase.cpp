#include "SpellBase.h"
#include "Net/UnrealNetwork.h"


void USpellBase::Server_CastSpell_Implementation(FVector TargetLocation)
{
	CurrentCooldown = Cooldown;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USpellBase::CountDownCooldown, 0.1f, true, 0.0f);

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
