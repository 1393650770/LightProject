// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LightProjectSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FRankItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName PlayerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerScore;

};

/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API ULightProjectSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = SaveGame)
	void MySaveGame();

	UFUNCTION(BlueprintCallable, Category = SaveGame)
	void MyReadGame();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FRankItem> PlayerData;
};
