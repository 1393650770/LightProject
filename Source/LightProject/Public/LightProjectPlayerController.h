// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LightProjectPlayerController.generated.h"

class AController;
class AActor;
/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API ALightProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	ALightProjectPlayerController();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Controller)
	TSubclassOf<AActor> PlayerStartClass; 
public:
	UFUNCTION(Server, Reliable)
	void RespawnPlayerEvent(AController* Controller);
};
