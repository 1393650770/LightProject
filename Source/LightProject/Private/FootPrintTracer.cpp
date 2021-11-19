// Fill out your copyright notice in the Description page of Project Settings.


#include "FootPrintTracer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "LightProject/LightProjectCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <Runtime/Experimental/Chaos/Public/Chaos/DebugDrawQueue.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void UFootPrintTracer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Player = MeshComp->GetOwner();
	if (Player)
	{
		MyCharacter = Cast<ALightProjectCharacter>(Player);
		if (MyCharacter)
		{
			LastLocation1 = MeshComp->GetSocketLocation(LeftFootSocketName);
			LastLocation2 = MeshComp->GetSocketLocation(RightFootSocketName);
		}
	}
}



void UFootPrintTracer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (Player&& MyCharacter&&MyCharacter->GetbIsPlayerSelf())
	{
		UWorld* World = Player->GetWorld();
		FVector HitDirection;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Player);
		QueryParams.bReturnPhysicalMaterial=true;
		QueryParams.bTraceComplex = true;
		if (!LeftFootSocketName.IsNone())
		{
			FVector NowLeftPosition = MeshComp->GetSocketLocation(LeftFootSocketName);

			HitDirection = NowLeftPosition - LastLocation1;
			HitResult.Empty();
			if(World->LineTraceMultiByChannel(HitResult, LastLocation1, NowLeftPosition,ECollisionChannel::ECC_Visibility,QueryParams))
			{ 

				if (HitResult.Num() > 0)
				{			
					FRotator SocketRotation= MeshComp->GetSocketRotation(LeftFootSocketName);
					FootPrintRotation.Pitch = -90;
					FootPrintRotation.Yaw = SocketRotation.Yaw;
					FootPrintRotation.Roll = SocketRotation.Roll;
					for (int i = 0; i < HitResult.Num(); i++)
					{
						AActor* HitActor = HitResult[i].GetActor();
						if (HitActor == Player)
						{
							continue;
						}

						if (FootPrintMatertial.Num() > 0)
						{

							UGameplayStatics::SpawnDecalAtLocation(World, FootPrintMatertial[0], FootPrintSize, HitResult[i].ImpactPoint, FootPrintRotation, FootPrintLifeSpan);
						}

						break;
					}
				}
			}
		}
		if (!RightFootSocketName.IsNone())
		{
			//µÚ¶þ¸ö²å²Û//
			HitResult.Empty();
			FVector NowRightPosition = MeshComp->GetSocketLocation(RightFootSocketName);

			HitDirection = NowRightPosition - LastLocation2;
			if (World->LineTraceMultiByChannel(HitResult, LastLocation2, NowRightPosition, ECollisionChannel::ECC_Visibility, QueryParams))
			{
				if (HitResult.Num() > 0)
				{
					FRotator SocketRotation = MeshComp->GetSocketRotation(RightFootSocketName);
					FootPrintRotation.Pitch = -90;
					FootPrintRotation.Yaw = SocketRotation.Yaw;
					FootPrintRotation.Roll = SocketRotation.Roll;
					for (int i = 0; i < HitResult.Num(); i++)
					{
						AActor* HitActor = HitResult[i].GetActor();
						if (HitActor == Player)
						{
							continue;
						}

						if (FootPrintMatertial.Num() > 0)
						{
							UGameplayStatics::SpawnDecalAtLocation(World, FootPrintMatertial[0], FootPrintSize, HitResult[i].ImpactPoint, FootPrintRotation, FootPrintLifeSpan);
						}

						break;
					}
				}
			}		
			if (FootSound.Num() > 0&&HitResult.Num()>0&& bIsPlaySound)
			{

				UGameplayStatics::PlaySoundAtLocation(World, FootSound[0], HitResult[0].ImpactPoint);
			}
		}

	}
	HitActors.Empty();
}