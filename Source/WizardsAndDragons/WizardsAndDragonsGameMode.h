#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizardsAndDragonsGameMode.generated.h"

class AAISpawner;
class AAIDragon;
class AWizardsAndDragonsCharacter;
class AWADGameState;

UCLASS(minimalapi)
class AWizardsAndDragonsGameMode : public AGameModeBase
{
	GENERATED_BODY()

		AWizardsAndDragonsGameMode();

public:
	// Every number of rounds the boss will show up
	UPROPERTY(EditDefaultsOnly)
	int BossWave = 5;

	//How many addtional enemies to spawn from start 
	int StartingNumberOfEnemies = 1;

	// Multiplier increase being added after each sucessful round
	UPROPERTY(EditDefaultsOnly)
	int WaveSpawnMultiplierIncrease = 0.2;

	//Starting value for spawn multiplier, scales with number of rounds
	UPROPERTY(EditDefaultsOnly)
	int InitialWaveSpawnMultiplier = 1;

	//Delay between dragon spawns
	UPROPERTY(EditDefaultsOnly)
	float DragonSpawnTimer = 5.0f;

	// Array of spawners
	UPROPERTY(EditAnywhere)
		TArray<AAISpawner*> AISpawners;

	//Array of dragons
	UPROPERTY(EditAnywhere)
		TArray<AAIDragon*> AIDragons;
	
	// Gamestate ref to updata score and wave number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
	AWADGameState* GameStateRef;

	// Start enemy spawner
	UFUNCTION(BlueprintCallable)
		void BeginSpawnEnemies();
	
	// Stop Enemies spawning by clearing timer
	UFUNCTION(BlueprintCallable)
		void StopSpawnEnemies();

	// Update the number of enemies spawned and add to array
	UFUNCTION(BlueprintCallable)
		void OnSpawnEnemy(AAIDragon* AIDragon);

	// Add score on death of enemy and remove from array
	UFUNCTION(BlueprintCallable)
		void OnEnemyDeath(AAIDragon* AIDragon, int ScoreValue);

	// Add Spawner to array 
	UFUNCTION(BlueprintCallable)
		void AddSpawner(AAISpawner* AISpawner);

protected:
	bool bSpawnEnemies = false;

	int CurrentWave = 0;

	int CurrentWaveNumberOfEnemies;

	int CurrentWaveNumberOfEnemiesSpawned;

	int CurrentWaveNumberOfEnemiesKilled;

	int CurrentWaveSpawnMultiplier = 1;

	int NumberOfSpawnedEnemies;

	virtual void BeginPlay() override;
	
	//On game over start over
	void OnGameOver();

	// Start new round
	UFUNCTION(BlueprintCallable)
		void BeginNewRound();

	void OnRoundOver();

	void OnRoundWin();

	// Start new round
	UFUNCTION(BlueprintCallable)
	void OnRoundLost();

	// Reset to starting values
	void ResetToStart();

	//Intermission in between rounds
	UPROPERTY(EditDefaultsOnly)
	float RoundIntermissionTime = 10.f;

	FTimerHandle RoundIntermissionTimerHandle;
};
