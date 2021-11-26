// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LightProjectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API ALightProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,Category = PlayerController)
	void OnPlayerIsDeath();
};
