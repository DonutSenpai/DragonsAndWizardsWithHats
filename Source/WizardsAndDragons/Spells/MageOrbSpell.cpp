#include "MageOrbSpell.h"

void UMageOrbSpell::InternalCastSpell(FVector TargetLocation)
{
	OnCastSpellEffect(TravelTime, TargetLocation);
	GetWorld()->GetTimerManager().SetTimer(HitExplosionHandle, FTimerDelegate::CreateUObject(this, &UMageOrbSpell::HitExplosion, TargetLocation), TravelTime, false);

}

void UMageOrbSpell::HitExplosion(FVector TargetLocation)
{
	TArray<AActor*> ExplosionTargets = GetSpellTargetsInRadius(TargetLocation);
	Server_DealDamage(ExplosionTargets);
}

