// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightProjectGameMode.h"
#include "LightProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
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

void ALightProjectGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareNextWave();
}

void ALightProjectGameMode::PlayerOver(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);		

	}
	OnMissionCompleted(InstigatorPawn);
}

void ALightProjectGameMode::StartWave()
{
	WaveCount++;
	NumsOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ALightProjectGameMode::SpawnBotTimerElapsed, 5.0f, true, 0.0f);
}

void ALightProjectGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	PrepareNextWave();
}

void ALightProjectGameMode::AlwaysEndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	GetWorldTimerManager().ClearTimer(TimerHandle_NextWaveStart);
}

void ALightProjectGameMode::PrepareNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ALightProjectGameMode::StartWave, 5.0f, false);;
}

void ALightProjectGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NumsOfBotsToSpawn--;

	if (NumsOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}
