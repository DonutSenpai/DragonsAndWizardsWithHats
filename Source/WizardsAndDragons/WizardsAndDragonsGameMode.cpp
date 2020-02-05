#include "WizardsAndDragonsGameMode.h"
#include "Player/WizardsAndDragonsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/AISpawner.h"
#include "Components/WADHealthComponent.h"
#include "Player/WizardsAndDragonsCharacter.h"

AWizardsAndDragonsGameMode::AWizardsAndDragonsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_WADPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AWizardsAndDragonsGameMode::BeginSpawnEnemies()
{
	for (AAISpawner* AISpawner : AISpawners)
	{
		AISpawner->StartSpawnDragonTimer(5.0f);
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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Number of Wave Enemies: %i"), CurrentWaveNumberOfEnemies));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spawned Enemies: %i"), CurrentWaveNumberOfEnemiesSpawned));

	if (CurrentWaveNumberOfEnemiesSpawned == CurrentWaveNumberOfEnemies)
	{
		bSpawnEnemies = false;
		StopSpawnEnemies();
	}
}

void AWizardsAndDragonsGameMode::OnEnemyDeath(AAIDragon* AIDragon)
{
	AIDragons.Remove(AIDragon);

	CurrentWaveNumberOfEnemiesKilled += 1;

	Score += 100;

	if (CurrentWaveNumberOfEnemiesKilled == CurrentWaveNumberOfEnemies)
	{
		OnRoundOver();
		OnRoundWin();
	}
}

void AWizardsAndDragonsGameMode::AddPlayer(AWizardsAndDragonsCharacter* PlayerCharacter)
{
	PlayerCharacters.Add(PlayerCharacter);
}

void AWizardsAndDragonsGameMode::AddSpawner(AAISpawner* AISpawner)
{
	AISpawners.Add(AISpawner);
}

void AWizardsAndDragonsGameMode::OnPlayerDeath()
{
	int NumberOfDeadPlayers = 0;

	for (AWizardsAndDragonsCharacter* PlayerChararcter : PlayerCharacters)
	{
		UWADHealthComponent* PlayerHealthComp;
		PlayerHealthComp = PlayerChararcter->FindComponentByClass<UWADHealthComponent>();

		if (PlayerHealthComp->IsDead())
			NumberOfDeadPlayers += 1;

		if (NumberOfDeadPlayers == PlayerCharacters.Num())
		{
			OnRoundOver();
			OnRoundLost();
		}
	}
}

void AWizardsAndDragonsGameMode::OnGameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game over, man. Game over!"));
}

void AWizardsAndDragonsGameMode::BeginNewRound()
{
	bSpawnEnemies = true;

	CurrentWave += 1;

	CurrentWaveSpawnMultiplier += WaveSpawnMultiplierIncrease;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurrentWave: %i"), CurrentWave));

	CurrentWaveNumberOfEnemies = StartingNumberOfEnemies + CurrentWave * CurrentWaveSpawnMultiplier;

	NumberOfSpawnedEnemies = 0;

	CurrentWaveNumberOfEnemiesKilled = 0;

	CurrentWaveNumberOfEnemiesSpawned = 0;

	if (CurrentWave % BossWave == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Boss Round"));
	}

	else
	{
		BeginSpawnEnemies();
	}

}

void AWizardsAndDragonsGameMode::OnRoundOver()
{
	bSpawnEnemies = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round Over"));
}

void AWizardsAndDragonsGameMode::OnRoundWin()
{
	Score += CurrentWave * 100;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round Won"));
	GetWorld()->GetTimerManager().SetTimer(RoundIntermissionTimerHandle, this, &AWizardsAndDragonsGameMode::BeginNewRound, RoundIntermissionTime, false);
}

void AWizardsAndDragonsGameMode::OnRoundLost()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Round Lost"));
}
