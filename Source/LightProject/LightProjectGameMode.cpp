// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightProjectGameMode.h"
#include "Net/UnrealNetwork.h"
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
	FirstWave();
}



/// <summary>
/// 当前波数开始
/// </summary>
void ALightProjectGameMode::StartWave()
{
	
	NumsOfBotsToSpawn += 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ALightProjectGameMode::SpawnBotTimerElapsed, TimeBetweenSPawnAI, true, 0.0f);
}


/// <summary>
/// 当前波数结束
/// </summary>
void ALightProjectGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);


	if (WaveCount < MaxWaveCount)
	{	
		WaveCount++;
		PrepareNextWave();
	}

}


/// <summary>
/// 永久结束波数
/// </summary>
void ALightProjectGameMode::AlwaysEndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	GetWorldTimerManager().ClearTimer(TimerHandle_NextWaveStart);
}


/// <summary>
///第一波开始
/// </summary>
void ALightProjectGameMode::FirstWave()
{
	if (WaveCount < MaxWaveCount)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_FirstWaveStart, this, &ALightProjectGameMode::StartWave, FirstWaveIntervalTime, false);
		WaveCount++;
		GetWorldTimerManager().SetTimer(TimerHandle_FirstWaveStartBluePrintEvent, this, &ALightProjectGameMode::FirstWaveStartOnCall, FirstWaveIntervalTime, false);
	}
}


/// <summary>
/// 准备开始下一波
/// </summary>
void ALightProjectGameMode::PrepareNextWave()
{
	if (WaveCount < MaxWaveCount)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ALightProjectGameMode::StartWave, WaveIntervalTime, false);;
	}
}


/// <summary>
/// 生成敌人的函数
/// </summary>
void ALightProjectGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NumsOfBotsToSpawn--;

	if (NumsOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

/// <summary>
/// 第一波函数产生时调用的定时函数
/// </summary>
void ALightProjectGameMode::FirstWaveStartOnCall()
{
	FirstWaveStartBlurPrint();
}
