// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightProjectGameMode.h"
#include "LightProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "LightProject.h"
#include <Kismet/GameplayStatics.h>

ALightProjectGameMode::ALightProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALightProjectGameMode::PlayerOver(APawn* InstigatorPawn)
{

	OnMissionCompleted(InstigatorPawn);
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);		

	}
}
