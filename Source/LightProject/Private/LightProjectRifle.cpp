// Fill out your copyright notice in the Description page of Project Settings.

#include "LightProjectRifle.h"
#include "Kismet/GameplayStatics.h"
#include "LightProject/LightProjectCharacter.h"
#include "LightProject/Public/LightProjectProjectile.h"

ALightProjectRifle::ALightProjectRifle()
{

}
void ALightProjectRifle::Fire()
{
	Super::Fire();
	UE_LOG(LogTemp, Log, TEXT("RifleFire"));
	return;
}
