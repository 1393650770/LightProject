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

	/// <summary>
	/// �ӵ�������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ALightProjectProjectile> ProjectileClass;

	/// <summary>
	/// �����תƫ��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FRotator CameraRotatorOffset;

	/// <summary>
	/// ����
	/// </summary>
	void Fire() override;
};
