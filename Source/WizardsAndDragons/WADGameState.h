#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WADGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverEvent);

class AWizardsAndDragonsCharacter;

UCLASS()
class WIZARDSANDDRAGONS_API AWADGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TArray<AWizardsAndDragonsCharacter*> PlayerCharacters;

	FGameOverEvent OnGameOver;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int Score = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int CurrentWave = 0;

	bool bPlayersAreDead = false;

	void AddScore(int ScoreToAdd);

	void ChangeCurrentWave(int NewWave);

	UFUNCTION(BlueprintCallable)
		void OnPlayerDeath();

	UFUNCTION(BlueprintCallable)
		void AddPlayer(AWizardsAndDragonsCharacter* PlayerCharacter);
};
