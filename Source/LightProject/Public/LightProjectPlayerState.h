// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include <LightProject/LightProjectCharacter.h>
#include "LightProjectPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API ALightProjectPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	float PlayerHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	int32 PlayerScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	EWeaponType PlayerWeaponType;

	UPROPERTY(BlueprintReadWrite, Category = Player)
	TArray<ALightProjectWeapon*> PlayerWeaponList;
};
