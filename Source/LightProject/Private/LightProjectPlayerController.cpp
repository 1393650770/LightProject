// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Controller.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Float16.h"
#include "Math/Float32.h"
#include <math.h>
#include <Runtime/Engine/Classes/GameFramework/PlayerStart.h>
#include "LightProject/LightProjectCharacter.h"

ALightProjectPlayerController::ALightProjectPlayerController()
{

}


/// <summary>
/// 服务器：重生玩家事件
/// </summary>
void ALightProjectPlayerController::RespawnPlayerEvent_Implementation(AController* Controller)
{
	Controller->GetPawn()->Destroyed();
	TArray<AActor*> OutActors;
	UWorld* World= GetWorld();
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), OutActors);
	int OutActorsIndex=  rand()%OutActors.Num();
	FTransform ActorTransform= OutActors[OutActorsIndex]->GetActorTransform();
	FActorSpawnParameters ActorSpawnParameters;
	ALightProjectCharacter* newPlayer= World->SpawnActor<ALightProjectCharacter>(ALightProjectCharacter::StaticClass(),ActorTransform, ActorSpawnParameters);
	if (newPlayer)
	{
		Controller->Possess(newPlayer);
	}
}
