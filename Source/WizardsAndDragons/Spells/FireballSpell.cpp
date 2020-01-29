#include "FireballSpell.h"

void UFireballSpell::InternalCastSpell(FVector TargetLocation)
{
	OnCastSpellEffect(TravelTime, TargetLocation);

	GetWorld()->GetTimerManager().SetTimer(HitExplosionHandle, FTimerDelegate::CreateUObject(this, &UFireballSpell::HitExplosion, TargetLocation), TravelTime, false);
}

void UFireballSpell::HitExplosion(FVector TargetLocation)
{
	TArray<AActor*> ExplosionTargets = GetSpellTargetsInRadius(TargetLocation);
	Server_DealDamage(ExplosionTargets);
}
