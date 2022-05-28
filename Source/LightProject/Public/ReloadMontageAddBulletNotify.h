// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ReloadMontageAddBulletNotify.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTPROJECT_API UReloadMontageAddBulletNotify : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	float QuerySurfaceTypeToChangeBaseDamage(const FHitResult& Hit);
public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	class AActor* Player;//当前玩家//

	//自带函数重写//
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)override;//Tick//

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;//通知结束//

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;//通知开始//

};
