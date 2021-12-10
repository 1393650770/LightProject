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
	/// ��ʾ��ǰ��Ϸģʽ�Ƿ���ȫ����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	bool bIsGameOver = false;

	/// <summary>
	/// ��һ�����˵Ķ�ʱ�����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_FirstWaveStart;

	/// <summary>
	/// ��һ�����˵���ͼ�¼���ʱ�����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_FirstWaveStartBluePrintEvent;

	/// <summary>
	/// ��һ�����˵���ͼ�¼���ʱ�����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_NextWaveStart;

	/// <summary>
	///�������ɵĶ�ʱ�����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "AIBot")
	FTimerHandle TimerHandle_BotSpawner;


	/// <summary>
	///�������ɵ�����
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 NumsOfBotsToSpawn=10;

	/// <summary>
	/// ÿ�����ɵ��˵Ļ�������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 BaseAddNumsOfBotsToSpawn = 10;

	/// <summary>
	/// ���˵������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 MaxWaveCount = 5;

	/// <summary>
	/// ��ǰ����
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	int32 WaveCount=0;

	/// <summary>
	/// ÿ��֮��ļ��ʱ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float WaveIntervalTime = 10.f;

	/// <summary>
	/// ��һ����ʼ�ļ��ʱ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float FirstWaveIntervalTime = 20.f;


	/// <summary>
	/// ���ɵ���֮��ļ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBot")
	float TimeBetweenSPawnAI = 2.0f;

	/// <summary>
	/// �Ƿ��Ѿ���Զֹͣ�����ɵ���
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "AIBot")
	bool bIsAlwaysStopWave = false;

	/// <summary>
	/// ��һ����ʼ����ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void FirstWaveStartBlurPrint();

	/// <summary>
	/// �����µĵ��˵���ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();
	
	/// <summary>
	/// ����������ҵ���ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void RespawnPlayerBluePrint(APlayerController* playercontroller);

	/// <summary>
	/// ��ǰ������ʼ
	/// </summary>
	void StartWave();

	/// <summary>
	/// ��ǰ��������
	/// </summary>
	void EndWave();

	/// <summary>
	/// ���ý�������
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AlwaysEndWave();

	/// <summary>
	///��һ����ʼ
	/// </summary>
	void FirstWave();

	/// <summary>
	/// ׼����ʼ��һ��
	/// </summary>
	void PrepareNextWave();


	/// <summary>
	/// ���ɵ��˵ĺ���
	/// </summary>
	void SpawnBotTimerElapsed();

	/// <summary>
	/// ��һ����������ʱ���õĶ�ʱ����
	/// </summary>
	void FirstWaveStartOnCall();

};



