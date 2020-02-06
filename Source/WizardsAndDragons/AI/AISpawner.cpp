#include "AISpawner.h"
#include "AIDragon.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnOnBeginPlay)
	{
		SpawnDragon();
	}
}

void AAISpawner::Destroyed()
{
	if (UWorld * World = GetWorld())
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::Destroyed();
}

void AAISpawner::SpawnDragonOverride(int32 InCount, float InDuration, bool bInRandomDuration, float InRandomDurationMin, float InRandomDurationMax)
{
	SpawnDragonImpl(InCount, InDuration, bInRandomDuration, InRandomDurationMin, InRandomDurationMax);
}

void AAISpawner::SpawnDragon()
{
	SpawnDragonImpl(Count, Duration, bRandomDuration, RandomDurationMin, RandomDurationMax);
}

void AAISpawner::SpawnDragonImpl(int32 InCount, float InDuration, bool bInRandomDuration, float InMinDuration, float InMaxDuration)
{
	for (int32 Index = 0; Index < InCount; ++Index)
	{
		StartSpawnDragonTimer(bInRandomDuration ? FMath::RandRange(InMinDuration, InMaxDuration) : InDuration);
	}
}

void AAISpawner::StartSpawnDragonTimer(float InDuration)
{
	if (InDuration > 0.0f)
	{

		GetWorld()->GetTimerManager().SetTimer(SpawnAITimerHandle, this, &AAISpawner::HandleSpawnTimerDone, InDuration, true);
	}
	else
	{
		HandleSpawnTimerDone();
	}
}

void AAISpawner::StopSpawnDragonTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnAITimerHandle);
}

void AAISpawner::ToggleBossWave(bool bBossRound)
{
	bBossWave = bBossRound;
}

void AAISpawner::HandleSpawnTimerDone()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (!bBossWave)
	{
		if (AAIDragon* SpawnedDragon = GetWorld()->SpawnActor<AAIDragon>(DragonClass, GetActorTransform(), Params))
		{
			BP_OnSpawned(SpawnedDragon);
		}
	}

	else
	{
		if (AAIDragon* SpawnedDragon = GetWorld()->SpawnActor<AAIDragon>(BossDragon, GetActorTransform(), Params))
		{
			BP_OnSpawned(SpawnedDragon);
		}
	}

}
