// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LightProjectGameMode.generated.h"


class ALightProjectCharacter;


UCLASS(minimalapi)
class ALightProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALightProjectGameMode();

	virtual void StartPlay() override;

	/// <summary>
	/// 表示当前游戏模式是否完全结束
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	bool bIsGameOver = false;

	/// <summary>
	/// 第一波敌人的定时器句柄
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_FirstWaveStart;

	/// <summary>
	/// 第一波敌人的蓝图事件定时器句柄
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_FirstWaveStartBluePrintEvent;

	/// <summary>
	/// 第一波敌人的蓝图事件定时器句柄
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_NextWaveStart;

	/// <summary>
	///敌人生成的定时器句柄
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_BotSpawner;


	/// <summary>
	///敌人生成的数量
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 NumsOfBotsToSpawn=10;

	/// <summary>
	/// 每次生成敌人的基础数量
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 BaseAddNumsOfBotsToSpawn = 10;

	/// <summary>
	/// 敌人的最大波数
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 MaxWaveCount = 5;

	/// <summary>
	/// 当前波数
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 WaveCount=0;

	/// <summary>
	/// 每波之间的间隔时间
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float WaveIntervalTime = 10.f;

	/// <summary>
	/// 第一波开始的间隔时间
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float FirstWaveIntervalTime = 20.f;


	/// <summary>
	/// 生成敌人之间的间隔
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float TimeBetweenSPawnAI = 2.0f;

	/// <summary>
	/// 是否已经永远停止了生成敌人
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "AIBot")
	bool bIsAlwaysStopWave = false;

	/// <summary>
	/// 第一波开始的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void FirstWaveStartBlurPrint();

	/// <summary>
	/// 生成新的敌人的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();
	
	/// <summary>
	/// 重新生成玩家的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void RespawnPlayerBluePrint(APlayerController* playercontroller);

	/// <summary>
	/// 当前波数开始
	/// </summary>
	void StartWave();

	/// <summary>
	/// 当前波数结束
	/// </summary>
	void EndWave();

	/// <summary>
	/// 永久结束波数
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AlwaysEndWave();

	/// <summary>
	///第一波开始
	/// </summary>
	void FirstWave();

	/// <summary>
	/// 准备开始下一波
	/// </summary>
	void PrepareNextWave();


	/// <summary>
	/// 生成敌人的函数
	/// </summary>
	void SpawnBotTimerElapsed();

	/// <summary>
	/// 第一波函数产生时调用的定时函数
	/// </summary>
	void FirstWaveStartOnCall();

};



