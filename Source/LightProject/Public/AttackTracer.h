// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackTracer.generated.h"

class ALightProjectCharacter;
/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API UAttackTracer : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	float QuerySurfaceTypeToChangeBaseDamage(const FHitResult& Hit);

	ALightProjectCharacter* MyCharacter=nullptr;
public:


	FCollisionQueryParams QueryParams;

	TArray<FHitResult>HitResult;//保存命中结果//

	TArray<AActor*>HitActors;//保存命中的Actor//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	AController* EventInstigator;//ApplyDamage函数参数//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	TSubclassOf<UDamageType>DamageTypeClass;//ApplyDamage函数参数//

	FVector LastLocation1;//插槽1位置//

	FVector LastLocation2;//插槽2位置//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	FName LeftSocketName=TEXT("HandSocketLeft");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	FName RightSocketName = TEXT("HandSocketRight");

	class AActor* Player;//当前玩家//

	class USkeletalMeshComponent* Weapon;//玩家武器//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	TArray<AActor*>ActorsToIgnore;//射线忽略目标//

	//自带函数重写//
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)override;//Tick//

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;//通知结束//

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;//通知开始//

};
