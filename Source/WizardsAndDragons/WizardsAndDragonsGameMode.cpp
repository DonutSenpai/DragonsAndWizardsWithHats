#include "WizardsAndDragonsGameMode.h"
#include "Player/WizardsAndDragonsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/AISpawner.h"
#include "WADGameState.h"

AWizardsAndDragonsGameMode::AWizardsAndDragonsGameMode()
{

}

void AWizardsAndDragonsGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentWaveSpawnMultiplier = InitialWaveSpawnMultiplier;

	GameStateRef->OnGameOver.AddDynamic(this, &AWizardsAndDragonsGameMode::OnRoundLost);
}

void AWizardsAndDragonsGameMode::BeginSpawnEnemies()
{
	for (AAISpawner* AISpawner : AISpawners)
	{
		AISpawner->StartSpawnDragonTimer(DragonSpawnTimer);
	}
}

void AWizardsAndDragonsGameMode::StopSpawnEnemies()
{
	for (AAISpawner* AISpawner : AISpawners)
	{
		AISpawner->StopSpawnDragonTimer();
	}
}

void AWizardsAndDragonsGameMode::OnSpawnEnemy(AAIDragon* AIDragon)
{
	AIDragons.Add(AIDragon);

	CurrentWaveNumberOfEnemiesSpawned += 1;

	if (CurrentWaveNumberOfEnemiesSpawned == CurrentWaveNumberOfEnemies)
	{
		bSpawnEnemies = false;
		StopSpawnEnemies();
	}
}

void AWizardsAndDragonsGameMode::OnEnemyDeath(AAIDragon* AIDragon, int ScoreValue)
{
	AIDragons.Remove(AIDragon);

	CurrentWaveNumberOfEnemiesKilled += 1;

	GameStateRef->AddScore(ScoreValue);

	if (CurrentWaveNumberOfEnemiesKilled == CurrentWaveNumberOfEnemies)
	{
		OnRoundOver();
		OnRoundWin();
	}
}

void AWizardsAndDragonsGameMode::AddSpawner(AAISpawner* AISpawner)
{
	AISpawners.Add(AISpawner);
}

void AWizardsAndDragonsGameMode::OnGameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game over, man. Game over!"));
}

void AWizardsAndDragonsGameMode::BeginNewRound()
{
	bSpawnEnemies = true;

	CurrentWave += 1;

	GameStateRef->ChangeCurrentWave(CurrentWave);

	if (CurrentWave % BossWave == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Boss Round"));

		for (AAISpawner* AISpawner : AISpawners)
		{
			AISpawner->ToggleBossWave(true);
		}
		CurrentWaveNumberOfEnemies = 1;

		NumberOfSpawnedEnemies = 0;

		CurrentWaveNumberOfEnemiesKilled = 0;

		CurrentWaveNumberOfEnemiesSpawned = 0;

		BeginSpawnEnemies();
	}

	else
	{
		for (AAISpawner* AISpawner : AISpawners)
		{
			AISpawner->ToggleBossWave(false);
		}

		CurrentWaveSpawnMultiplier += WaveSpawnMultiplierIncrease;

		CurrentWaveNumberOfEnemies = StartingNumberOfEnemies + CurrentWave * CurrentWaveSpawnMultiplier;

		NumberOfSpawnedEnemies = 0;

		CurrentWaveNumberOfEnemiesKilled = 0;

		CurrentWaveNumberOfEnemiesSpawned = 0;

		BeginSpawnEnemies();
	}
}

void AWizardsAndDragonsGameMode::OnRoundOver()
{
	bSpawnEnemies = false;
	StopSpawnEnemies();
}

void AWizardsAndDragonsGameMode::OnRoundWin()
{
	GameStateRef->AddScore(CurrentWave * 100);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round Won"));
	GetWorld()->GetTimerManager().SetTimer(RoundIntermissionTimerHandle, this, &AWizardsAndDragonsGameMode::BeginNewRound, RoundIntermissionTime, false);
}

void AWizardsAndDragonsGameMode::OnRoundLost()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round Lost"));

	GetWorld()->GetTimerManager().SetTimer(RoundIntermissionTimerHandle, this, &AWizardsAndDragonsGameMode::ResetToStart, RoundIntermissionTime, false);
}


// Reset to starting values before starting over
void AWizardsAndDragonsGameMode::ResetToStart()
{
	CurrentWave = 0;

	CurrentWaveSpawnMultiplier = InitialWaveSpawnMultiplier;

	CurrentWaveNumberOfEnemies = StartingNumberOfEnemies + CurrentWave * CurrentWaveSpawnMultiplier;

	NumberOfSpawnedEnemies = 0;

	CurrentWaveNumberOfEnemiesKilled = 0;

	CurrentWaveNumberOfEnemiesSpawned = 0;
}