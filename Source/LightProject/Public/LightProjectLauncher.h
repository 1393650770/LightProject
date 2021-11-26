// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LightProjectWeapon.h"
#include "LightProjectLauncher.generated.h"

class ALightProjectProjectile;
class FLifetimeProperty;
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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ALightProjectProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FRotator CameraRotatorOffset;

	void Fire() override;
};
