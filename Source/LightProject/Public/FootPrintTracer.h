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
	/// �Ƿ����ڲ�������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Sound)
	bool bIsPlaySound=false	;

	/// <summary>
	/// �Ų���������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TArray <USoundBase*> FootSound;

	/// <summary>
	/// ��ӡ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootMatertial)
	TArray<UMaterialInterface* > FootPrintMatertial;

	/// <summary>
	/// ��ӡ��С
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootMatertial)
	FVector FootPrintSize;

	/// <summary>
	/// ��ӡ��ת
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootMatertial)
	FRotator FootPrintRotation;
	

	/// <summary>
	/// ��ӡ��������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = FootMatertial)
	float FootPrintLifeSpan = 5.0f;

	TArray<FHitResult>HitResult;//�������н��//

	TArray<AActor*>HitActors;//�������е�Actor//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	AController* EventInstigator;//ApplyDamage��������//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	TSubclassOf<UDamageType>DamageTypeClass;//ApplyDamage��������//

	FVector LastLocation1;//���1λ��//

	FVector LastLocation2;//���2λ��//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	FName LeftFootSocketName = TEXT("FootSocketLeft");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	FName RightFootSocketName = TEXT("FootSocketRight");

	class AActor* Player;//��ǰ���//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootTrace)
	TArray<AActor*>ActorsToIgnore;//���ߺ���Ŀ��//


		//�Դ�������д//

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;//֪ͨ����//

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;//֪ͨ��ʼ//

};
