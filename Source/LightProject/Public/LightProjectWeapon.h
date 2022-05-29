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
	/// 服务器：子弹数增加
	/// </summary>
	UFUNCTION(Server, Reliable)
	void AddBulletNum(int num);



protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// 攻击力基数
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = Weapon)
	float Attack=20.0f;

	/// <summary>
	/// 子弹随机偏移的最大半径
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = Weapon)
	float BulletSpread = 10.f;
	
	/// <summary>
	/// 当前的子弹数
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = Weapon)
	int CurrentBulletNum = 30.f;

	/// <summary>
	/// 最大的子弹数
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = Weapon)
	int MaxBulletNum = 30.f;


	/// <summary>
	/// 当前武器的类型
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = Weapon)
	EWeaponType WeaponType = EWeaponType::Rifle;

	/// <summary>
	/// 伤害类型
	/// </summary>
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= Weapon)
	TSubclassOf<UDamageType> DamageType;


	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly,Category=Components)
	USkeletalMeshComponent* MeshComp;

	/// <summary>
	/// 装弹的蒙太奇
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	UAnimMontage* ReloadMontage;

	/// <summary>
	/// 装弹的蒙太奇播放的Section
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	FName ReloadSectionName;

	/// <summary>
	/// 子弹路径粒子特效
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly,Category = WeaponEffect)
	UParticleSystem* TraceEmitter;
	
	/// <summary>
	/// 子弹爆炸粒子特效
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponEffect)
	UParticleSystem* ExplosionEffect;

	/// <summary>
	/// 子弹击中生火的特效
	/// </summary>
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = WeaponEffect)
	UParticleSystem* FireEffect;

	/// <summary>
	/// 特效产生起点的SocketName
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleSoketName = TEXT("MuzzleSocket");

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Weapon)
	FName TraceTargetName = TEXT("Target");

	/// <summary>
	/// 武器拥有者
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Replicated, Category = Weapon)
	EWeaponMaster WeaponMaster = EWeaponMaster::None;

	/// <summary>
	/// 开火声音
	/// </summary>
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = WeaponSound)
	USoundBase* FireSound;

	/// <summary>
	/// 是否是持续开火
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = Fire)
	bool bIsFireAlways = false;

	/// <summary>
	/// 是否是持续开火
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = Fire)
	bool bIsCanFire = true;

	/// <summary>
	/// 每次开火的间隔
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fire)
	float FireCooldownTime = 0.2f;

	/// <summary>
	/// 开会间隔的定时器句柄
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Replicated,Category = Fire)
	FTimerHandle TimerHandle_FireCooldown;

	/// <summary>
	/// 暂停开火
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Fire)
	void ComeInCooldownFire();

	/// <summary>
	/// 改变bIsFire到true
	/// </summary>
	void ChangebIsCanFireToTrue();

	/// <summary>
	/// 开火
	/// </summary>
	UFUNCTION(BlueprintCallable,  Category=Weapon)
	virtual void Fire();

	/// <summary>
	/// 射线检测
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual FVector RayTraceShootingSight();

	/// <summary>
	/// 获取子弹发射方向偏移（弃用）
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual FRotator GetProjectileRotation(FHitResult& Hit, bool& result);

	/// <summary>
	/// 服务器：播放爆炸特效
	/// </summary>
	UFUNCTION(Server,Reliable)
	void PlayExplosionEffectServer(FVector Positon, FRotator Rotation);

	/// <summary>
	/// 多播：播放爆炸特效
	/// </summary>
	UFUNCTION(NetMulticast, Unreliable)
	void PlayExplosionEffectMulticast(FVector Positon, FRotator Rotation);

	/// <summary>
	/// 服务器：播放开火和轨迹特效
	/// </summary>
	UFUNCTION(Server, Reliable)
	void PlayFireAndTraceEffectServer(FVector Positon);

	/// <summary>
	/// 多播：播放开火和轨迹特效
	/// </summary>
	UFUNCTION(NetMulticast, Unreliable)
	void PlayFireAndTraceEffectMulticast(FVector Positon);

};
