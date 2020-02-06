#include "WADGameState.h"
#include "Player/WizardsAndDragonsCharacter.h"
#include "Components/WADHealthComponent.h"
#include "Net/UnrealNetwork.h"


void AWADGameState::AddScore(int ScoreToAdd)
{
	Score += ScoreToAdd;
}

void AWADGameState::ChangeCurrentWave(int NewWave)
{
	CurrentWave = NewWave;
}

void AWADGameState::AddPlayer(AWizardsAndDragonsCharacter* PlayerCharacter)
{
	PlayerCharacters.Add(PlayerCharacter);
	PlayerCharacter->HealthComponent->OnDeath.AddDynamic(this, &AWADGameState::OnPlayerDeath);
}

void AWADGameState::OnPlayerDeath()
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
			OnGameOver.Broadcast();
		}
	}
}

void AWADGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWADGameState, Score);
	DOREPLIFETIME(AWADGameState, CurrentWave);
}

