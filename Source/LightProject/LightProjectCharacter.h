// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LightProjectCharacter.generated.h"

class ALightProjectWeapon;
class FFloat16;
class AActor;
class UAnimMontage;

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	Fist=0,
	Rifle=1,
	Launcher=2
};

UENUM(BlueprintType)
enum class EWeaponMaster :uint8
{
	Default=0,
	None=10,
	Player=11,
	Enemy=12
};


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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsGameOver = false;

	UFUNCTION(BlueprintCallable, Category = Player)
	void CreateDefaultShootWeapon();

	UFUNCTION()
	void MouseTurn(float Val);

	UFUNCTION()
	void MouseLookUp(float Val);

	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeToFreeView();

	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeLeaveFreeViewTick(float DeltaTime);


	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeToIronSightTick(float DeltaTime);

	UFUNCTION(BlueprintCallable,Server,Reliable,Category = Player)
	void ChangeToIronSight();

	UFUNCTION()
	void StartCrouch();
	UFUNCTION()
	void EndCrouch();

	virtual FVector GetPawnViewLocation() const;

	UFUNCTION(Server,Reliable)
	void MouseTurnRotation();

	UFUNCTION(NetMulticast, Unreliable)
	void MouseTurnRoationMuticast();

	UFUNCTION()
	void OnRepHealthUpdate();

	void OnHealthUpdate();

	UFUNCTION(BlueprintCallable, Category = Player)
	void CheckGroundObjects();

	UFUNCTION(BlueprintImplementableEvent ,Category = Player)
	void OnCheckGroundObjects(FHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnCheckGroundNoObjects();

	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnChangerCharacterHealth();


	void CallGameModeRespawnPlayerClient();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float ClientYaw = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float ClientPitch = 0.0f;

	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeHealth(float value) ;

	UFUNCTION(BlueprintCallable, Category = Player)
	void AddHealth(float value);

	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangePlayerTotalDamage(int value);
	
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void ChangePlayerTotalDamageBluePrint(int value);

	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToPercent() const;

	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToFloat() const;

	UFUNCTION(BlueprintCallable, Category = Player)
	bool GetbIsIronsight() const;

	UFUNCTION(BlueprintCallable, Category = Player)
	bool GetbIsPlayerSelf() const;
/// <summary>
/// 自定义的protected属性
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Player)
	bool bIsPlayerSelf = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	bool  bIsAuthority = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	FName PlayerName = TEXT("Player");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRepHealthUpdate, Category = Player)
	float Health = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Player)
	int32 PlayerTotalDamage = 0;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	ALightProjectWeapon* Weapon=nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsIronsight = false;

	float TargetFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsFreeView = false;

	bool bIsChangeAwayFromFreeView = false;

	FRotator FreeViewRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	FTimerHandle TimerHandle_ReSpawnerPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float DefaultFOV =50.0f;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = Player)
	float IronsightFOV = 50.0f;
		
	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = Player)
	float IronsightInterpSpeed = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated,Category = Player)
	EWeaponType WeaponType = EWeaponType::Fist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	int32 DetectObjectDistance = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	float DetectObjectRadius = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<TEnumAsByte<EObjectTypeQuery> > DetectObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<AActor*> DetectObjectActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Player)
	TArray<FWeaponSlot> WeaponList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anim)
	TArray<UAnimMontage*> AnimMontageArray;

};

