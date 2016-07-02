// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Trapped.h"
#include "TrappedGameMode.h"
#include "TrappedCharacter.h"

ATrappedGameMode::ATrappedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
