// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectLauncher.h"

ALightProjectLauncher::ALightProjectLauncher()
{
	Attack = 50.0f;
	WeaponType = EWeaponType::Launcher;;
}

void ALightProjectLauncher::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("LaunchFire"));
	return;
}
