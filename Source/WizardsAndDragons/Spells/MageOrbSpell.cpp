#include "MageOrbSpell.h"

void UMageOrbSpell::InternalCastSpell(FVector TargetLocation)
{
	OnCastSpellEffectInternal(TravelTime, TargetLocation);
	GetWorld()->GetTimerManager().SetTimer(HitExplosionHandle, FTimerDelegate::CreateUObject(this, &UMageOrbSpell::HitExplosion, TargetLocation), TravelTime, false);

}

void UMageOrbSpell::InternalDealDamage(const TArray<AActor*>& SpellTargets)
{
	//Deal no damage here, doing it in the spawned MageOrb
}

void UMageOrbSpell::OnCastSpellEffectInternal_Implementation(float TimeToTravel, FVector TargetLocation /*= FVector::ZeroVector*/)
{
	OnCastSpellEffect(TravelTime, TargetLocation);
}

void UMageOrbSpell::HitExplosion(FVector TargetLocation)
{
	TArray<AActor*> ExplosionTargets = GetSpellTargetsInRadius(TargetLocation);
	Server_DealDamage(ExplosionTargets);
}

