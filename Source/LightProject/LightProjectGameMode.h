// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LightProjectGameMode.generated.h"


UENUM(BlueprintType)
enum class EGameState :uint8
{
	Start,
	MainMenu,
	SinglePlay,
	MultyPlay,
	GameOver,
};

class ALightProjectCharacter;

UCLASS(minimalapi)
class ALightProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALightProjectGameMode();

	virtual void StartPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void EventStartPlay();

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	TArray<ALightProjectCharacter*> SpawnBotArray;

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_FirstWaveStart;

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_NextWaveStart;

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_BotSpawner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 NumsOfBotsToSpawn=10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 BasrAddNumsOfBotsToSpawn = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 MaxWaveCount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 WaveCount=0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float WaveIntervalTime = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float FirstWaveIntervalTime = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float TimeBetweenSPawnAI = 2.0f;

	UPROPERTY(BlueprintReadOnly, Category = "AIBot")
	bool bIsAlwaysStopWave = false;

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	bool bIsGameOver = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void FinishGame();

	void StartWave();

	void EndWave();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AlwaysEndWave();

	void FirstWave();

	void PrepareNextWave();

	void SpawnBotTimerElapsed();




};



