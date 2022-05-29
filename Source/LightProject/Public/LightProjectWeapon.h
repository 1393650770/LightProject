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
class FLifetimeProperty;



UCLASS()
class LIGHTPROJECT_API ALightProjectWeapon : public AActor 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightProjectWeapon();

	/// <summary>
	/// ���������ӵ�������
	/// </summary>
	UFUNCTION(Server, Reliable)
	void AddBulletNum(int num);



protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// ����������
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = Weapon)
	float Attack=20.0f;

	/// <summary>
	/// �ӵ����ƫ�Ƶ����뾶
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = Weapon)
	float BulletSpread = 10.f;
	
	/// <summary>
	/// ��ǰ���ӵ���
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = Weapon)
	int CurrentBulletNum = 30.f;

	/// <summary>
	/// �����ӵ���
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = Weapon)
	int MaxBulletNum = 30.f;


	/// <summary>
	/// ��ǰ����������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = Weapon)
	EWeaponType WeaponType = EWeaponType::Rifle;

	/// <summary>
	/// �˺�����
	/// </summary>
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= Weapon)
	TSubclassOf<UDamageType> DamageType;


	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly,Category=Components)
	USkeletalMeshComponent* MeshComp;

	/// <summary>
	/// װ������̫��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	UAnimMontage* ReloadMontage;

	/// <summary>
	/// װ������̫�沥�ŵ�Section
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	FName ReloadSectionName;

	/// <summary>
	/// �ӵ�·��������Ч
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly,Category = WeaponEffect)
	UParticleSystem* TraceEmitter;
	
	/// <summary>
	/// �ӵ���ը������Ч
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponEffect)
	UParticleSystem* ExplosionEffect;

	/// <summary>
	/// �ӵ������������Ч
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = WeaponEffect)
	UParticleSystem* FireEffect;

	/// <summary>
	/// ��Ч��������SocketName
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleSoketName = TEXT("MuzzleSocket");

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Weapon)
	FName TraceTargetName = TEXT("Target");

	/// <summary>
	/// ����ӵ����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Replicated, Category = Weapon)
	EWeaponMaster WeaponMaster = EWeaponMaster::None;

	/// <summary>
	/// ��������
	/// </summary>
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = WeaponSound)
	USoundBase* FireSound;

	/// <summary>
	/// �Ƿ��ǳ�������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = Fire)
	bool bIsFireAlways = false;

	/// <summary>
	/// �Ƿ��ǳ�������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = Fire)
	bool bIsCanFire = true;

	/// <summary>
	/// ÿ�ο���ļ��
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fire)
	float FireCooldownTime = 0.2f;

	/// <summary>
	/// �������Ķ�ʱ�����
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Replicated,Category = Fire)
	FTimerHandle TimerHandle_FireCooldown;

	/// <summary>
	/// ��ͣ����
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Fire)
	void ComeInCooldownFire();

	/// <summary>
	/// �ı�bIsFire��true
	/// </summary>
	void ChangebIsCanFireToTrue();

	/// <summary>
	/// ����
	/// </summary>
	UFUNCTION(BlueprintCallable,  Category=Weapon)
	virtual void Fire();

	/// <summary>
	/// ���߼��
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual FVector RayTraceShootingSight();

	/// <summary>
	/// ��ȡ�ӵ����䷽��ƫ�ƣ����ã�
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual FRotator GetProjectileRotation(FHitResult& Hit, bool& result);

	/// <summary>
	/// �����������ű�ը��Ч
	/// </summary>
	UFUNCTION(Server,Reliable)
	void PlayExplosionEffectServer(FVector Positon, FRotator Rotation);

	/// <summary>
	/// �ಥ�����ű�ը��Ч
	/// </summary>
	UFUNCTION(NetMulticast, Unreliable)
	void PlayExplosionEffectMulticast(FVector Positon, FRotator Rotation);

	/// <summary>
	/// �����������ſ���͹켣��Ч
	/// </summary>
	UFUNCTION(Server, Reliable)
	void PlayFireAndTraceEffectServer(FVector Positon);

	/// <summary>
	/// �ಥ�����ſ���͹켣��Ч
	/// </summary>
	UFUNCTION(NetMulticast, Unreliable)
	void PlayFireAndTraceEffectMulticast(FVector Positon);

};
