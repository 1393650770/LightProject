// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <LightProject/LightProjectCharacter.h>
#include "LightProjectWeapon.generated.h"


class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;

UCLASS()
class LIGHTPROJECT_API ALightProjectWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightProjectWeapon();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float Attack=20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	EWeaponType WeaponType = EWeaponType::Rifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= Weapon)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Components)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Weapon)
	UParticleSystem* TraceEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* ExplosionEffect;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleSoketName = TEXT("MuzzleSocket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName TraceTargetName = TEXT("Target");

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	EWeaponMaster WeaponMaster = EWeaponMaster::None;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	bool bIsFireAlways = false;


	UFUNCTION(BlueprintCallable, Category=Weapon)
	virtual void Fire();
public:	


};
