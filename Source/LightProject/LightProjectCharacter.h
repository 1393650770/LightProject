// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LightProjectCharacter.generated.h"

class ALightProjectWeapon;
class FFloat16;
class AActor;

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	Fist,
	Rifle,
	Launcher
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsGameOver = false;

	UFUNCTION()
	void MouseTurn(float Val);



	UFUNCTION(BlueprintCallable, Category = Player)
	void CreateDefaultShootWeapon();
	UFUNCTION()
	void StartCrouch();
	UFUNCTION()
	void EndCrouch();

	virtual FVector GetPawnViewLocation() const;

	UFUNCTION(BlueprintCallable, Category = Player)
	void CheckGroundObjects();

	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnCheckGroundObjects(FHitResult Hit,bool& Result);

	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnChangerCharacterHealth();

/// <summary>
/// 自定义的public方法/属性
/// </summary>
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangeHealth(float value) ;

	UFUNCTION(BlueprintCallable, Category = Player)
	void ChangePlayerTotalDamage(int value);

	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToPercent() const;

	UFUNCTION(BlueprintCallable, Category = Player)
	float GetHealthToFloat() const;
/// <summary>
/// 自定义的protected属性
/// </summary>
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	FName PlayerName = TEXT("Player");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	float Health = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	int32 PlayerTotalDamage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	ALightProjectWeapon* Weapon=nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	EWeaponType WeaponType = EWeaponType::Fist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	int32 DetectObjectDistance = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	float DetectObjectRadius = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<TEnumAsByte<EObjectTypeQuery> > DetectObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerDetectObject)
	TArray<AActor*> DetectObjectActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	TArray<FWeaponSlot> WeaponList;
};

