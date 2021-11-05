// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackTracer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "LightProject/LightProjectCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
void UAttackTracer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Player = MeshComp->GetOwner();
	if (Player)
	{
		ALightProjectCharacter* MyCharacter = Cast<ALightProjectCharacter>(Player);
		if (MyCharacter)
		{
			LastLocation1= MeshComp->GetSocketLocation(LeftSocketName);
			LastLocation2 = MeshComp->GetSocketLocation(RightSocketName);
		}
	}
}


void UAttackTracer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	
	if (Player)
	{
		float HitBaseDamage = rand() % 20 + 1;
		UWorld* World = Player->GetWorld();
		FVector NowLeftPosition = MeshComp->GetSocketLocation(LeftSocketName);
		FVector NowRightPosition = MeshComp->GetSocketLocation(RightSocketName);
		FVector HitDirection = NowLeftPosition - LastLocation1;
		UKismetSystemLibrary::LineTraceMulti(World, LastLocation1, NowLeftPosition, ETraceTypeQuery::TraceTypeQuery4, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
		//UKismetSystemLibrary::BoxTraceMulti(World, LastLocation1, MeshComp->GetSocketLocation(LeftSocketName), FVector(5, 30, 50), MeshComp->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery4, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);
		
		//World->LineTraceMultiByChannel(HitResult, LastLocation1, NowLeftPosition, ECC_Visibility, QueryParams)
		for (int i = 0; i < HitResult.Num(); i++)
		{
			AActor* HitActor = HitResult[i].GetActor();//��ȡ�������߻��е�Actor//
			if (!HitActors.Contains(HitActor))//��ѯ�������Ƿ��б��λ��е�Actor�����û������ӽ����鲢�����Դ��˺���������ֹһ��֪ͨ�ڶ�λ��е����//
			{
				HitActors.Add(HitActor);
				HitBaseDamage = QuerySurfaceTypeToChangeBaseDamage(HitResult[i]);
				UGameplayStatics::ApplyPointDamage(HitActor, HitBaseDamage, HitDirection, HitResult[i], EventInstigator, Player, DamageTypeClass);
				//UGameplayStatics::ApplyDamage(HitActor, 20.f, EventInstigator, Player, DamageTypeClass);
			}
		}
		//�ڶ������//
		UKismetSystemLibrary::LineTraceMulti(World, LastLocation2, NowRightPosition, ETraceTypeQuery::TraceTypeQuery4, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
		//UKismetSystemLibrary::BoxTraceMulti(World, LastLocation2, MeshComp->GetSocketLocation(RightSocketName), FVector(5, 30, 50), MeshComp->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery4, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);
		
		HitDirection = NowRightPosition - LastLocation2;
		for (int i = 0; i < HitResult.Num(); i++)
		{
			AActor* HitActor = HitResult[i].GetActor();
			if (!HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);
				HitBaseDamage = QuerySurfaceTypeToChangeBaseDamage(HitResult[i]);
				UGameplayStatics::ApplyPointDamage(HitActor, HitBaseDamage, HitDirection, HitResult[i], EventInstigator, Player, DamageTypeClass);
				//UGameplayStatics::ApplyDamage(HitActor, 20.f, EventInstigator, Player, DamageTypeClass);
			}
		}
		//һ��Tick������µ�ǰ��۵�λ�ñ�������һ������洢�ı�������ֵ//
		LastLocation1 = MeshComp->GetSocketLocation(LeftSocketName);
		LastLocation2 = MeshComp->GetSocketLocation(RightSocketName);

	}


}

void UAttackTracer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	HitActors.Empty();
}

float UAttackTracer::QuerySurfaceTypeToChangeBaseDamage(const FHitResult& Hit)
{
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	float HitBaseDamage= rand() % 20 + 1;
	switch (HitSurfaceType)
	{
	case SurfaceType_Default:
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=SurfaceType_Default"));
		break;
	case SurfaceType1://SurfaceType1=ActorBody
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorBody"));
		HitBaseDamage = rand() % 20 + 1;
		break;
	case SurfaceType2://SurfaceType2=ActorHead
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorHead"));
		HitBaseDamage = rand() % 20 + 81;
		break;
	case SurfaceType3://SurfaceType3=ActorFoot
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorFoot"));
		HitBaseDamage = rand() % 10 + 1;
		break;
	case SurfaceType4://SurfaceType4=ActorHand
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorHand"));
		HitBaseDamage = rand() % 10 + 1;
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=Error"));
		break;
	}
	return HitBaseDamage;
}
