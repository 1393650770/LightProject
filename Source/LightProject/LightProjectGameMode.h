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



UCLASS(minimalapi)
class ALightProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALightProjectGameMode();

	virtual void StartPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_NextWaveStart;

	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_BotSpawner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 NumsOfBotsToSpawn=0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 WaveCount=0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float TimeBetweenWaves = 2.0f;

	UPROPERTY(BlueprintReadOnly, Category = "AIBot")
	bool bIsAlwaysStopWave = false;
		
	void PlayerOver(APawn* InstigatorPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void StartWave();

	void EndWave();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AlwaysEndWave();

	void PrepareNextWave();

	void SpawnBotTimerElapsed();

};



