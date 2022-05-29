// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LightProjectCharacter.generated.h"

class ALightProjectWeapon;
class FFloat16;
class AActor;
class UAnimMontage;

//Weapon������

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	Fist=0,
	Rifle=1,
	Launcher=2
};

//Weapon��������

UENUM(BlueprintType)
enum class EWeaponMaster :uint8
{
	Default=0,
	None=10,
	Player=11,
	Enemy=12
};

//Weapon���ԵĽṹ��

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
/// �Զ����Protected����
/// </summary>
protected:

	/// <summary>
	/// �Ƿ�����Ϸ���׽�����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsGameOver = false;

	/// <summary>
	/// ����Ĭ�ϵ�����
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void CreateDefaultShootWeapon();

	UFUNCTION()
	void MouseTurn(float Val);

	UFUNCTION()
	void MouseLookUp(float Val);

	/// <summary>
	/// �ı�Ϊ�����ӽ�
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeToFreeView();

	/// <summary>
	/// �ı�Ϊ�����ӽǵ�Tick
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeLeaveFreeViewTick(float DeltaTime);

	/// <summary>
	/// �ı䵽��׼״̬��Tick
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeToIronSightTick(float DeltaTime);

	/// <summary>
	/// �ı䵽��׼״̬
	/// </summary>
	UFUNCTION(BlueprintCallable,Server,Reliable,Category = Player)
	void ChangeToIronSight();

	/// <summary>
	/// �����׷�
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Player)
	void StartCrouch();

	/// <summary>
	/// �����׷�
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Player)
	void EndCrouch();

	/// <summary>
	/// ���صĻ�ȡPawn���ӽ�λ��
	/// </summary>
	/// <returns></returns>
	virtual FVector GetPawnViewLocation() const;

	/// <summary>
	/// �����������������ת��ɫ�ӽ�
	/// </summary>
	UFUNCTION(Server,Reliable)
	void MouseTurnRotation();

	/// <summary>
	/// �㲥�����������ת��ɫ�ӽ�
	/// </summary>
	UFUNCTION(NetMulticast, Unreliable)
	void MouseTurnRoationMuticast();

	UFUNCTION()
	void OnRepHealthUpdate();

	/// <summary>
	/// ����ɫ����ʱ����
	/// </summary>
	void OnHealthUpdate();

	/// <summary>
	/// ����������
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void CheckGroundObjects();


	/// <summary>
	/// ���������巢�����������ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent ,Category = Player)
	void OnCheckGroundObjects(FHitResult Hit);

	/// <summary>
	/// ���������巢��û���������ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnCheckGroundNoObjects();

	/// <summary>
	/// ���ı��ɫѪ������ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnChangerCharacterHealth();

	/// <summary>
	/// �ͻ��ˣ�������Ϸģʽ��������ҵĺ���
	/// </summary>
	void CallGameModeRespawnPlayerClient();


	/// <summary>
	/// ��������������Ϸģʽ��������ҵĺ���
	/// </summary>
	UFUNCTION(Server,Reliable)
	void CallGameModeRespawnPlayer();


/// <summary>
/// �Զ����public����/����
/// </summary>
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// ��ɫ����תYaw
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float ClientYaw = 0.0f;


	/// <summary>
	/// ��ɫ����תPitch
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float ClientPitch = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated,Category = Player)
	bool IsReloading = false;


	/// <summary>
	/// �ı�����ֵ��ֻ�ܼ��٣�
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeHealth(float value) ;


	/// <summary>
	/// �ı�����ֵ��ֻ�����ӣ�
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void AddHealth(float value);

	/// <summary>
	/// �ı��ɫ��ɵ��˺�����
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangePlayerTotalDamage(int value);
	

	/// <summary>
	/// �ı��ɫ���˺���������ͼ�¼�
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void ChangePlayerTotalDamageBluePrint(int value);

	/// <summary>
	/// ��ȡ����ɫ����ֵ��ת��Ϊ�ٷֱ�
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToPercent() const;

	/// <summary>
	/// ��ȡ������ֵ
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToFloat() const;

	/// <summary>
	/// ��ȡ��bIsIronsight
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	bool GetbIsIronsight() const;

	/// <summary>
	/// ��ȡ��bIsPlayerSelf
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	bool GetbIsPlayerSelf() const;

	/// <summary>
	/// ��ȡ����ɫ��ǰ������
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	ALightProjectWeapon* GetWeapon() const;


	/// <summary>
	/// ��ȡ����ɫ��ǰ����������
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Player)
	EWeaponType GetWeaponType() const;


	/// <summary>
	/// �����������������װ�ӵ��ĺ���
	/// </summary>
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void PlayReloadMontageServerByCharacter(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);
	void PlayReloadMontageServerByCharacter_Implementation(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	/// <summary>
	/// �ಥ�����������װ�ӵ��ĺ���
	/// </summary>
	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void PlayReloadMontageMulticastByCharacter(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);
	void PlayReloadMontageMulticastByCharacter_Implementation(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	/// <summary>
	/// ������������reloading Ϊfalse
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetReloadFalseServerByCharacter();
	void SetReloadFalseServerByCharacter_Implementation();



/// <summary>
/// �Զ����protected����
/// </summary>
protected:

	/// <summary>
	/// �Ƿ������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Player)
	bool bIsPlayerSelf = false;

	/// <summary>
	/// �Ƿ��Ǵ������ã�
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	bool  bIsAuthority = false;

	/// <summary>
	/// �������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	FName PlayerName = TEXT("Player");

	/// <summary>
	/// ��ǰ����ֵ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRepHealthUpdate, Category = Player)
	float Health = 100.0f;

	/// <summary>
	/// �������ֵ
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Player)
	float MaxHealth = 100.0f;

	/// <summary>
	/// �����ɵ��˺�����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Player)
	int32 PlayerTotalDamage = 0;


	/// <summary>
	/// ��ɫ��ǰ������
	/// </summary>
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	ALightProjectWeapon* Weapon=nullptr;

	/// <summary>
	/// �Ƿ�����׼״̬
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsIronsight = false;

	/// <summary>
	/// Ŀ���FOV
	/// </summary>
	float TargetFOV;

	/// <summary>
	/// �Ƿ��������ӽ�
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsFreeView = false;

	/// <summary>
	/// �Ƿ�������ӽǸı�
	/// </summary>
	bool bIsChangeAwayFromFreeView = false;


	FRotator FreeViewRotation;

	/// <summary>
	/// �������ɽ�ɫ�Ķ�ʱ�����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	FTimerHandle TimerHandle_ReSpawnerPlayer;


	/// <summary>
	/// Ĭ�ϵ�FOV
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float DefaultFOV =50.0f;

	/// <summary>
	/// ��׼״̬�µ�FOV
	/// </summary>
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = Player)
	float IronsightFOV = 50.0f;
	
	/// <summary>
	/// ��׼״̬FOV���л��ٶ�
	/// </summary>
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = Player)
	float IronsightInterpSpeed = 50.0f;

	/// <summary>
	/// ��ǰ��������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated,Category = Player)
	EWeaponType WeaponType = EWeaponType::Fist;
	
	/// <summary>
	/// �������ľ���
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	int32 DetectObjectDistance = 200;


	/// <summary>
	/// �������ľ���
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	float DetectObjectRadius = 20.0f;
	
	/// <summary>
	/// ������������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<TEnumAsByte<EObjectTypeQuery> > DetectObjectTypes;

	/// <summary>
	/// �����Ե�Actors
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<AActor*> DetectObjectActorsToIgnore;

	/// <summary>
	/// ��ǰӵ�е������б�
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	TArray<FWeaponSlot> WeaponList;

	/// <summary>
	/// ������̫���б�
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anim)
	TArray<UAnimMontage*> AnimMontageArray;

};

