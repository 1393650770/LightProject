// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FootPrintTracer.generated.h"

class ALightProjectCharacter;
class USoundBase;
class UMaterialInterface;

/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API UFootPrintTracer : public UAnimNotifyState
{
	GENERATED_BODY()


public:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/// <summary>
	/// 是否正在播放声音
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Sound)
	bool bIsPlaySound=false	;

	/// <summary>
	/// 脚步声音集合
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TArray <USoundBase*> FootSound;

	/// <summary>
	/// 脚印材质
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootMatertial)
	TArray<UMaterialInterface* > FootPrintMatertial;

	/// <summary>
	/// 脚印大小
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootMatertial)
	FVector FootPrintSize;

	/// <summary>
	/// 脚印旋转
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootMatertial)
	FRotator FootPrintRotation;
	

	/// <summary>
	/// 脚印生命周期
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = FootMatertial)
	float FootPrintLifeSpan = 5.0f;

	TArray<FHitResult>HitResult;//保存命中结果//

	TArray<AActor*>HitActors;//保存命中的Actor//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	AController* EventInstigator;//ApplyDamage函数参数//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	TSubclassOf<UDamageType>DamageTypeClass;//ApplyDamage函数参数//

	FVector LastLocation1;//插槽1位置//

	FVector LastLocation2;//插槽2位置//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	FName LeftFootSocketName = TEXT("FootSocketLeft");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	FName RightFootSocketName = TEXT("FootSocketRight");

	class AActor* Player;//当前玩家//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	TArray<AActor*>ActorsToIgnore;//射线忽略目标//


		//自带函数重写//

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;//通知结束//

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;//通知开始//

};
