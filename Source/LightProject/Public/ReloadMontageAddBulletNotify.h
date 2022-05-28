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

	class AActor* Player;//��ǰ���//

	//�Դ�������д//
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)override;//Tick//

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;//֪ͨ����//

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;//֪ͨ��ʼ//

};
