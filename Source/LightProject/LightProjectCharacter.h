// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LightProjectCharacter.generated.h"

class ALightProjectWeapon;
class FFloat16;
class AActor;
class UAnimMontage;

//Weapon的类型

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	Fist=0,
	Rifle=1,
	Launcher=2
};

//Weapon的所有者

UENUM(BlueprintType)
enum class EWeaponMaster :uint8
{
	Default=0,
	None=10,
	Player=11,
	Enemy=12
};

//Weapon属性的结构体

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY() 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ALightProjectWeapon* Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName SlotName;
};

UCLASS(config=Game)
class ALightProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ALightProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	ALightProjectWeapon* BPWeapon;

	bool bIsStartTouch = false;
	FVector FFirstTouchPoint;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchRepeated(ETouchIndex::Type FingerIndex, FVector Location);
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

/// <summary>
/// 自定义的Protected方法
/// </summary>
protected:

	/// <summary>
	/// 是否是游戏彻底结束了
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsGameOver = false;

	/// <summary>
	/// 创建默认的武器
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void CreateDefaultShootWeapon();

	UFUNCTION()
	void MouseTurn(float Val);

	UFUNCTION()
	void MouseLookUp(float Val);

	/// <summary>
	/// 改变为自由视角
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeToFreeView();

	/// <summary>
	/// 改变为自由视角的Tick
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeLeaveFreeViewTick(float DeltaTime);

	/// <summary>
	/// 改变到瞄准状态的Tick
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeToIronSightTick(float DeltaTime);

	/// <summary>
	/// 改变到瞄准状态
	/// </summary>
	UFUNCTION(BlueprintCallable,Server,Reliable,Category = Player)
	void ChangeToIronSight();

	/// <summary>
	/// 开启蹲伏
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Player)
	void StartCrouch();

	/// <summary>
	/// 结束蹲伏
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Player)
	void EndCrouch();

	/// <summary>
	/// 重载的获取Pawn的视角位置
	/// </summary>
	/// <returns></returns>
	virtual FVector GetPawnViewLocation() const;

	/// <summary>
	/// 服务器：根据鼠标旋转角色视角
	/// </summary>
	UFUNCTION(Server,Reliable)
	void MouseTurnRotation();

	/// <summary>
	/// 广播：根据鼠标旋转角色视角
	/// </summary>
	UFUNCTION(NetMulticast, Unreliable)
	void MouseTurnRoationMuticast();

	UFUNCTION()
	void OnRepHealthUpdate();

	/// <summary>
	/// 当角色更新时调用
	/// </summary>
	void OnHealthUpdate();

	/// <summary>
	/// 检查地面物体
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void CheckGroundObjects();


	/// <summary>
	/// 检查地面物体发现有物体的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent ,Category = Player)
	void OnCheckGroundObjects(FHitResult Hit);

	/// <summary>
	/// 检查地面物体发现没有物体的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnCheckGroundNoObjects();

	/// <summary>
	/// 当改变角色血量的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnChangerCharacterHealth();

	/// <summary>
	/// 客户端：调用游戏模式的重生玩家的函数
	/// </summary>
	void CallGameModeRespawnPlayerClient();


	/// <summary>
	/// 服务器：调用游戏模式的重生玩家的函数
	/// </summary>
	UFUNCTION(Server,Reliable)
	void CallGameModeRespawnPlayer();


/// <summary>
/// 自定义的public方法/属性
/// </summary>
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// 角色的旋转Yaw
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float ClientYaw = 0.0f;


	/// <summary>
	/// 角色的旋转Pitch
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float ClientPitch = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated,Category = Player)
	bool IsReloading = false;


	/// <summary>
	/// 改变生命值（只能减少）
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeHealth(float value) ;


	/// <summary>
	/// 改变生命值（只能增加）
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void AddHealth(float value);

	/// <summary>
	/// 改变角色造成的伤害总量
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangePlayerTotalDamage(int value);
	

	/// <summary>
	/// 改变角色的伤害总量的蓝图事件
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void ChangePlayerTotalDamageBluePrint(int value);

	/// <summary>
	/// 获取到角色生命值并转换为百分比
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToPercent() const;

	/// <summary>
	/// 获取到生命值
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToFloat() const;

	/// <summary>
	/// 获取到bIsIronsight
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	bool GetbIsIronsight() const;

	/// <summary>
	/// 获取到bIsPlayerSelf
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	bool GetbIsPlayerSelf() const;

	/// <summary>
	/// 获取到角色当前的武器
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	ALightProjectWeapon* GetWeapon() const;


	/// <summary>
	/// 获取到角色当前的武器类型
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	EWeaponType GetWeaponType() const;


	/// <summary>
	/// 服务器：调用玩家重装子弹的函数
	/// </summary>
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void PlayReloadMontageServerByCharacter(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);
	void PlayReloadMontageServerByCharacter_Implementation(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	/// <summary>
	/// 多播：调用玩家重装子弹的函数
	/// </summary>
	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void PlayReloadMontageMulticastByCharacter(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);
	void PlayReloadMontageMulticastByCharacter_Implementation(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	/// <summary>
	/// 服务器：设置reloading 为false
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetReloadFalseServerByCharacter();
	void SetReloadFalseServerByCharacter_Implementation();



/// <summary>
/// 自定义的protected属性
/// </summary>
protected:

	/// <summary>
	/// 是否是玩家
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Player)
	bool bIsPlayerSelf = false;

	/// <summary>
	/// 是否是代理（弃用）
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	bool  bIsAuthority = false;

	/// <summary>
	/// 玩家名称
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	FName PlayerName = TEXT("Player");

	/// <summary>
	/// 当前生命值
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRepHealthUpdate, Category = Player)
	float Health = 100.0f;

	/// <summary>
	/// 最大生命值
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Player)
	float MaxHealth = 100.0f;

	/// <summary>
	/// 玩家造成的伤害总量
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Player)
	int32 PlayerTotalDamage = 0;


	/// <summary>
	/// 角色当前的武器
	/// </summary>
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	ALightProjectWeapon* Weapon=nullptr;

	/// <summary>
	/// 是否在瞄准状态
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsIronsight = false;

	/// <summary>
	/// 目标的FOV
	/// </summary>
	float TargetFOV;

	/// <summary>
	/// 是否在自由视角
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsFreeView = false;

	/// <summary>
	/// 是否从自由视角改变
	/// </summary>
	bool bIsChangeAwayFromFreeView = false;


	FRotator FreeViewRotation;

	/// <summary>
	/// 重新生成角色的定时器句柄
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	FTimerHandle TimerHandle_ReSpawnerPlayer;


	/// <summary>
	/// 默认的FOV
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float DefaultFOV =50.0f;

	/// <summary>
	/// 瞄准状态下的FOV
	/// </summary>
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = Player)
	float IronsightFOV = 50.0f;
	
	/// <summary>
	/// 瞄准状态FOV的切换速度
	/// </summary>
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = Player)
	float IronsightInterpSpeed = 50.0f;

	/// <summary>
	/// 当前武器类型
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated,Category = Player)
	EWeaponType WeaponType = EWeaponType::Fist;
	
	/// <summary>
	/// 检查物体的距离
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	int32 DetectObjectDistance = 200;


	/// <summary>
	/// 检查物体的距离
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	float DetectObjectRadius = 20.0f;
	
	/// <summary>
	/// 检测的物体类型
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<TEnumAsByte<EObjectTypeQuery> > DetectObjectTypes;

	/// <summary>
	/// 检测忽略的Actors
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<AActor*> DetectObjectActorsToIgnore;

	/// <summary>
	/// 当前拥有的武器列表
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	TArray<FWeaponSlot> WeaponList;

	/// <summary>
	/// 动画蒙太奇列表
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anim)
	TArray<UAnimMontage*> AnimMontageArray;

};

