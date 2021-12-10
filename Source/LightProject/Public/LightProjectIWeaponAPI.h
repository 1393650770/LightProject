// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LightProject/LightProjectCharacter.h"
#include "LightProject/Public/LightProjectWeapon.h"
#include "LightProjectIWeaponAPI.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class  ULightProjectIWeaponAPI :public UInterface
{
	GENERATED_BODY()
};

class ILightProjectIWeaponAPI
{
	GENERATED_BODY()

public:

	/// <summary>
	/// 获取Weapon的蓝图事件接口
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	ALightProjectWeapon*  GetWeaponType(EWeaponType& WeaponType,FName& SocketName);
};