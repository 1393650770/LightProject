// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LightProjectWeapon.h"
#include "LightProjectLauncher.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API ALightProjectLauncher : public ALightProjectWeapon
{
	GENERATED_BODY()
public:
	ALightProjectLauncher();

protected:

	void Fire() override;
};
