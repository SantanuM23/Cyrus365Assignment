// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cyrus365AssignmentGameMode.h"
#include "Cyrus365AssignmentCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACyrus365AssignmentGameMode::ACyrus365AssignmentGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
