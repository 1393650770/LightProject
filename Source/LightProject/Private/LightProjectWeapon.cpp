// Fill out your copyright notice in the Description page of Project Settings.

#include "LightProjectWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Experimental/Chaos/Public/Chaos/DebugDrawQueue.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Chaos/ChaosEngineInterface.h"
#include "GameFramework/Character.h"
#include "LightProject/LightProjectCharacter.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>


// Sets default values
ALightProjectWeapon::ALightProjectWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void ALightProjectWeapon::ComeInCooldownFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_FireCooldown);
	FTimerDelegate ChangebIsCanFireToTrueDelegate = FTimerDelegate::CreateUObject(this, &ALightProjectWeapon::ChangebIsCanFireToTrue);
	GetWorldTimerManager().SetTimer(TimerHandle_FireCooldown, ChangebIsCanFireToTrueDelegate, FireCooldownTime, false);
	bIsCanFire = false;
}

void ALightProjectWeapon::ChangebIsCanFireToTrue()
{
	bIsCanFire = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_FireCooldown);
}

void ALightProjectWeapon::Fire()
{
	if (!bIsCanFire)
	{
		return;
	}
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	AActor* const MyOwner = GetOwner();
	UWorld* const World = GetWorld();
	APawn* const MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	ALightProjectCharacter* PlayerCharacter= Cast<ALightProjectCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (MyOwner->GetVelocity().Size() > 1.0f && PlayerCharacter->bIsCrouched)
	{
		return;	
	}
	float MyOwnerActorRotationYaw = MyOwner->GetActorRotation().Yaw;

	if (abs(MyOwnerActorRotationYaw +360- MyPawn->GetViewRotation().Yaw) > 91.0f&& abs(MyOwnerActorRotationYaw  - MyPawn->GetViewRotation().Yaw) > 91.0f)
		return;

	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShootDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + ShootDirection * 10000.0f;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;


		FVector TraceEndPoint = TraceEnd;

		FHitResult Hit;


		if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();

			float HitBaseDamage = rand() % (int)Attack + 1;

			EPhysicalSurface HitSurfaceType= UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			switch (HitSurfaceType)
			{
				case SurfaceType_Default:
					UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=SurfaceType_Default"));
					break;
				case SurfaceType1://SurfaceType1=ActorBody
					UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorBody"));
					HitBaseDamage = rand() % (int)Attack + 1;
					break;
				case SurfaceType2://SurfaceType2=ActorHead
					UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorHead"));
					HitBaseDamage = rand() % (int)Attack + 81;
					break;
				case SurfaceType3://SurfaceType3=ActorFoot
					UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorFoot"));
					HitBaseDamage = rand() % ((int)Attack/2) + 1;
					break;
				case SurfaceType4://SurfaceType4=ActorHand
					UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=ActorHand"));
					HitBaseDamage = rand() % ((int)Attack/2) + 1;
					break;
				default:
					UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=Error"));
					break;
			}
			if (ExplosionEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			
			UGameplayStatics::ApplyPointDamage(HitActor, HitBaseDamage, ShootDirection,Hit,MyOwner->GetInstigatorController(),this, DamageType);

			TraceEndPoint = Hit.ImpactPoint;
		}

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSoketName);

		if (FireEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, FireEffect, MuzzleLocation);
		}

		if (TraceEmitter)
		{
			UParticleSystemComponent* ParticleSystemComponent= UGameplayStatics::SpawnEmitterAtLocation(World, TraceEmitter, MuzzleLocation);
			if (ParticleSystemComponent)
			{
				ParticleSystemComponent->SetVectorParameter(TraceTargetName, TraceEndPoint);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Don't Get ParticleSystemComponent"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Don't Get TraceEmitter"));
		}
		ComeInCooldownFire();
	}
}

FVector ALightProjectWeapon::RayTraceShootingSight()
{
	AActor* const MyOwner = GetOwner();

	UWorld* const World = GetWorld();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector ShootDirection = EyeRotation.Vector();
	FVector TraceEnd = EyeLocation + ShootDirection * 10000.0f;
	if (!MyOwner)
		return TraceEnd;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);

	FHitResult Hit;


	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		return Hit.ImpactPoint;
	}
	return TraceEnd;
}

FRotator ALightProjectWeapon::GetProjectileRotation(FHitResult& Hit,bool& result)
{
	FRotator FinalRotator = FRotator::ZeroRotator;
	AActor* const MyOwner = GetOwner();
	if (!MyOwner)
		return FinalRotator;
	UWorld* const World = GetWorld();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FVector SelfLocation= MeshComp->GetSocketLocation(MuzzleSoketName);
	FRotator SelfRotation=MeshComp->GetComponentRotation();
	FVector TraceEnd = RayTraceShootingSight();

	if (World->LineTraceSingleByChannel(Hit, SelfLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		TraceEnd = Hit.ImpactPoint;
		result = true;
	}

	FinalRotator = UKismetMathLibrary::FindLookAtRotation(SelfLocation, TraceEnd);
	return FinalRotator;
}
