// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <LightProject/LightProjectCharacter.h>
#include "LightProjectWeapon.generated.h"


class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;
class USoundBase;



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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = WeaponEffect)
	UParticleSystem* TraceEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponEffect)
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponEffect)
	UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleSoketName = TEXT("MuzzleSocket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName TraceTargetName = TEXT("Target");

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	EWeaponMaster WeaponMaster = EWeaponMaster::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSound)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fire)
	bool bIsFireAlways = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fire)
	bool bIsCanFire = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fire)
	float FireCooldownTime = 0.2f;

	UPROPERTY(BlueprintReadWrite, Category = Fire)
	FTimerHandle TimerHandle_FireCooldown;

	UFUNCTION(BlueprintCallable, Category = Fire)
	void ComeInCooldownFire();

	void ChangebIsCanFireToTrue();

	UFUNCTION(BlueprintCallable, Category=Weapon)
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual FVector RayTraceShootingSight();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual FRotator GetProjectileRotation(FHitResult& Hit, bool& result);
};
