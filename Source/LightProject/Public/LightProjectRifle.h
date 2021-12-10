// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LightProjectWeapon.h"
#include "LightProjectRifle.generated.h"

class ALightProjectProjectile;
class FLifetimeProperty;
/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API ALightProjectRifle : public ALightProjectWeapon
{
	GENERATED_BODY()
public:
	ALightProjectRifle();
protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// ¿ª»á
	/// </summary>
	void Fire() override;
};
