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

	TArray<FHitResult>HitResult;//�������н��//

	TArray<AActor*>HitActors;//�������е�Actor//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	AController* EventInstigator;//ApplyDamage��������//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	TSubclassOf<UDamageType>DamageTypeClass;//ApplyDamage��������//

	FVector LastLocation1;//���1λ��//

	FVector LastLocation2;//���2λ��//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	FName LeftSocketName=TEXT("HandSocketLeft");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	FName RightSocketName = TEXT("HandSocketRight");

	class AActor* Player;//��ǰ���//

	class USkeletalMeshComponent* Weapon;//�������//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AttackTrace)
	TArray<AActor*>ActorsToIgnore;//���ߺ���Ŀ��//

	//�Դ�������д//
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)override;//Tick//

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;//֪ͨ����//

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;//֪ͨ��ʼ//

};
