#include "FireballSpell.h"

void UFireballSpell::InternalCastSpell(FVector TargetLocation)
{
	OnCastSpellEffectInternal(TravelTime, TargetLocation);
	GetWorld()->GetTimerManager().SetTimer(HitExplosionHandle, FTimerDelegate::CreateUObject(this, &UFireballSpell::HitExplosion, TargetLocation), TravelTime, false);
}

void UFireballSpell::OnCastSpellEffectInternal_Implementation(float TimeToTravel, FVector TargetLocation /*= FVector::ZeroVector*/)
{
	OnCastSpellEffect(TravelTime, TargetLocation);

}

void UFireballSpell::HitExplosion(FVector TargetLocation)
{
	TArray<AActor*> ExplosionTargets = GetSpellTargetsInRadius(TargetLocation);
	Server_DealDamage(ExplosionTargets);
}
