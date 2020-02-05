#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizardsAndDragonsGameMode.generated.h"

class AAISpawner;
class AAIDragon;
class AWizardsAndDragonsCharacter;

UCLASS(minimalapi)
class AWizardsAndDragonsGameMode : public AGameModeBase
{
	GENERATED_BODY()

		AWizardsAndDragonsGameMode();

public:
	int BossWave = 5;

	int StartingNumberOfEnemies = 1;

	int WaveSpawnMultiplierIncrease = 0.2;

	UPROPERTY(EditAnywhere)
		TArray<AWizardsAndDragonsCharacter*> PlayerCharacters;

	UPROPERTY(EditAnywhere)
		TArray<AAISpawner*> AISpawners;

	UPROPERTY(EditAnywhere)
		TArray<AAIDragon*> AIDragons;

	UFUNCTION(BlueprintCallable)
		void BeginSpawnEnemies();

	UFUNCTION(BlueprintCallable)
		void StopSpawnEnemies();

	UFUNCTION(BlueprintCallable)
		void OnSpawnEnemy(AAIDragon* AIDragon);

	UFUNCTION(BlueprintCallable)
		void OnEnemyDeath(AAIDragon* AIDragon);

	UFUNCTION(BlueprintCallable)
		void AddPlayer(AWizardsAndDragonsCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
		void AddSpawner(AAISpawner* AISpawner);

	UFUNCTION(BlueprintCallable)
		void OnPlayerDeath();

protected:
	bool bPlayersAreDead = false;

	bool bSpawnEnemies = false;

	int CurrentWaveNumberOfEnemies;

	int CurrentWaveNumberOfEnemiesSpawned;

	int CurrentWaveNumberOfEnemiesKilled;

	int CurrentWaveSpawnMultiplier = 1;

	UPROPERTY(BlueprintReadOnly)
	int Score = 0;

	int CurrentWave = 0;

	int NumberOfSpawnedEnemies;

	void OnGameOver();

	UFUNCTION(BlueprintCallable)
		void BeginNewRound();

	void OnRoundOver();

	void OnRoundWin();

	void OnRoundLost();

	float RoundIntermissionTime = 10.f;

	FTimerHandle RoundIntermissionTimerHandle;

};



