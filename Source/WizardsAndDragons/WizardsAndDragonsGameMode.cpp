// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WizardsAndDragonsGameMode.h"
#include "Player/WizardsAndDragonsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWizardsAndDragonsGameMode::AWizardsAndDragonsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_WADPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
