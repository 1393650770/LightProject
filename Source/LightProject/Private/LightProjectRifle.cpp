// Fill out your copyright notice in the Description page of Project Settings.

#include "LightProjectRifle.h"
#include "Kismet/GameplayStatics.h"
#include "LightProject/LightProjectCharacter.h"
#include "LightProject/Public/LightProjectProjectile.h"

ALightProjectRifle::ALightProjectRifle()
{

}
void ALightProjectRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}
void ALightProjectRifle::Fire()
{
	if (!bIsCanFire)
	{
		return;
	}
	Super::Fire();
	ComeInCooldownFire();
	return;
}
