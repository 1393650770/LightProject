// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "LightProject/LightProjectCharacter.h"
#include "LightProject/Public/LightProjectProjectile.h"
ALightProjectLauncher::ALightProjectLauncher()
{
	Attack = 50.0f;
	WeaponType = EWeaponType::Launcher;;
}

void ALightProjectLauncher::Fire()
{
	if (!bIsCanFire)
	{
		return;
	}

	AActor* LaunchOwner = GetOwner();
	UWorld* World = GetWorld();
	if (LaunchOwner)
	{
		APawn* OwnerPawn = Cast<APawn>(LaunchOwner);
		if (OwnerPawn)
		{
			FHitResult Hit;
			bool HitResult = false;
			const FVector SpawnLocation =  MeshComp->GetSocketLocation(MuzzleSoketName);
			const FRotator SpawnRotation = GetProjectileRotation(Hit, HitResult);
			
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = OwnerPawn;
			ActorSpawnParams.Owner = this;
			World->SpawnActor<ALightProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			ComeInCooldownFire();
		}
	}
	return;
}
