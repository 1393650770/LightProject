// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackTracer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "LightProject/LightProjectCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "LightProjectTeamComponent.h"
#include "Net/UnrealNetwork.h"


void UAttackTracer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Player = MeshComp->GetOwner();
	if (Player)
	{
		MyCharacter = Cast<ALightProjectCharacter>(Player);
		if (MyCharacter)
		{
			LastLocation1= MeshComp->GetSocketLocation(LeftSocketName);
			LastLocation2 = MeshComp->GetSocketLocation(RightSocketName);
		}
	}
}


void UAttackTracer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams SharedParams;
	SharedParams.Condition = ELifetimeCondition::COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttackTracer, Weapon, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttackTracer, Player, SharedParams);
}

void UAttackTracer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	
	if (Player)
	{
		float HitBaseDamage = rand() % 20 + 1;
		UWorld* World = Player->GetWorld();
		
		FVector HitDirection;
		if (!LeftSocketName.IsNone())
		{
			FVector NowLeftPosition = MeshComp->GetSocketLocation(LeftSocketName);
			HitDirection = NowLeftPosition - LastLocation1;
			UKismetSystemLibrary::LineTraceMulti(World, LastLocation1, NowLeftPosition, ETraceTypeQuery::TraceTypeQuery4, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

			for (int i = 0; i < HitResult.Num(); i++)
			{
				AActor* HitActor = HitResult[i].GetActor();//��ȡ�������߻��е�Actor//
				if (!HitActors.Contains(HitActor))//��ѯ�������Ƿ��б��λ��е�Actor�����û������ӽ����鲢�����Դ��˺���������ֹһ��֪ͨ�ڶ�λ��е����//
				{
					if (Player != HitActor)
					{
						HitActors.Add(HitActor);
						HitBaseDamage = QuerySurfaceTypeToChangeBaseDamage(HitResult[i]);
						if (!ULightProjectTeamComponent::IsFriendlyTeam(HitActor, Player))
						{
							UGameplayStatics::ApplyPointDamage(HitActor, HitBaseDamage, HitDirection, HitResult[i], MyCharacter->GetController(), Player, DamageTypeClass);
						}
					}
				}
			}
			//һ��Tick������µ�ǰ��۵�λ�ñ�������һ������洢�ı�������ֵ//
			LastLocation1 = MeshComp->GetSocketLocation(LeftSocketName);
		}
		if (!RightSocketName.IsNone())
		{
			//�ڶ������//
			FVector NowRightPosition = MeshComp->GetSocketLocation(RightSocketName);
			UKismetSystemLibrary::LineTraceMulti(World, LastLocation2, NowRightPosition, ETraceTypeQuery::TraceTypeQuery4, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
			HitDirection = NowRightPosition - LastLocation2;
			for (int i = 0; i < HitResult.Num(); i++)
			{
				AActor* HitActor = HitResult[i].GetActor();
				if (!HitActors.Contains(HitActor))
				{
					if (Player != HitActor)
					{
						HitActors.Add(HitActor);
						HitBaseDamage = QuerySurfaceTypeToChangeBaseDamage(HitResult[i]);
						if (!ULightProjectTeamComponent::IsFriendlyTeam(HitActor, Player))
						{
							UGameplayStatics::ApplyPointDamage(HitActor, HitBaseDamage, HitDirection, HitResult[i], MyCharacter->GetController(), Player, DamageTypeClass);
						}
						//UGameplayStatics::ApplyDamage(HitActor, 20.f, EventInstigator, Player, DamageTypeClass);
					}
				}
			}
			//һ��Tick������µ�ǰ��۵�λ�ñ�������һ������洢�ı�������ֵ//
			LastLocation2 = MeshComp->GetSocketLocation(RightSocketName);
		}

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
		break;
	case SurfaceType1://SurfaceType1=ActorBody
		HitBaseDamage = rand() % 20 + 1;
		break;
	case SurfaceType2://SurfaceType2=ActorHead
		HitBaseDamage = rand() % 20 + 81;
		break;
	case SurfaceType3://SurfaceType3=ActorFoot
		HitBaseDamage = rand() % 10 + 1;
		break;
	case SurfaceType4://SurfaceType4=ActorHand
		HitBaseDamage = rand() % 10 + 1;
		break;
	default:
		break;
	}
	return HitBaseDamage;
}
