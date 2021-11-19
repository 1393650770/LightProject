// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GameFramework/Actor.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "LightProject/LightProjectCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "LightProjectWeapon.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>




// Sets default values
ALightProjectProjectile::ALightProjectProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

		// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ALightProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	// Die after 3 seconds by default
	InitialLifeSpan = 10.0f;
}



void ALightProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	ChangeBounciness(Hit);
	
	if ((Cast<ALightProjectCharacter>(OtherActor) && Cast<ALightProjectCharacter>(OtherActor)->GetbIsPlayerSelf()) || Cast<ALightProjectWeapon>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Error : OnHitPlayer"));
		return;
	}

	if (HitSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	float CurrentSpeed = FMath::Abs(UKismetMathLibrary::VSize(GetVelocity()));
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{



		if ((Cast<APawn>(OtherActor) || CurrentSpeed <= LeastSpeedLimit) && (OtherActor != this))
		{
			UWorld* World = GetWorld();
			//UKismetSystemLibrary::SphereOverlapActors(this, Hit.ImpactPoint, BrokenSphereRadius, BrokenObjectTypes, BrokenActorClassFilter, BrokenActorsToIgnore, BrokenOutActors);
			UKismetSystemLibrary::SphereOverlapComponents(this, Hit.ImpactPoint, BrokenSphereRadius, BrokenObjectTypes, BrokenActorClassFilter, BrokenActorsToIgnore, BrokenOutComponents);

			FVector BrokenDirection = GetVelocity();
			FVector ProjectileLocation = GetActorLocation();
			AActor* HurtActor = nullptr;
			float HurtBaseDamage = 80.0f;
			int OutComponentsSize = BrokenOutComponents.Num();

			if (OutComponentsSize > 0)
			{
				for (int i = 0; i < OutComponentsSize; i++)
				{
					HurtActor = nullptr;
					HurtActor = BrokenOutComponents[i]->GetAttachmentRootActor();
					if (Cast<ALightProjectCharacter>(HurtActor) && Cast<ALightProjectCharacter>(HurtActor)->GetbIsPlayerSelf())
					{
						continue;
					}
					if (BrokenOutComponents[i]->IsSimulatingPhysics() && BrokenOutComponents[i]->Mobility == EComponentMobility::Movable)
					{
						BrokenDirection = (BrokenOutComponents[i]->GetComponentLocation() - ProjectileLocation);
						BrokenOutComponents[i]->AddImpulseAtLocation(BrokenDirection * BrokenImpuseSize, BrokenOutComponents[i]->GetComponentLocation());

					}
					
					if (HurtActor && HurtActor->CanBeDamaged())
					{

						HurtBaseDamage = (BrokenSphereRadius*1.3f-(BrokenOutComponents[i]->GetComponentLocation() - ProjectileLocation).Size() / BrokenSphereRadius) * BrokenDamage;
						UGameplayStatics::ApplyPointDamage(HurtActor, HurtBaseDamage, BrokenDirection, Hit,GetOwner()->GetInstigatorController(), this, DamageType);
					}
				}
			}
			Broken();
			return;
		}
	}
	if (CurrentSpeed <= LeastSpeedLimit)
	{
		Broken();
		return;
	}
	MakeNoise(1.0f, GetInstigator());
}


void ALightProjectProjectile::Broken()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	if (BrokenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BrokenSound, GetActorLocation());
	}
	Destroy();
	return;
}

void ALightProjectProjectile::ChangeBounciness(const FHitResult& Hit)
{
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	switch (HitSurfaceType)
	{
	case SurfaceType_Default:
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=SurfaceType_Default"));
		ProjectileMovement->Bounciness = 0.6f;
		ProjectileMovement->Friction = 0.2f;
		break;
	case SurfaceType5://SurfaceType1=EnvironmentSoil
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=EnvironmentSoil"));
		ProjectileMovement->Bounciness = 0.2f;
		ProjectileMovement->Friction = 0.8f;
		break;
	case SurfaceType6://SurfaceType2=EnvironmentIron
		UE_LOG(LogTemp, Log, TEXT("HitSurfaceType=EnvironmentIron"));
		ProjectileMovement->Bounciness = 0.85f;
		ProjectileMovement->Friction = 0.1f;
		break;
	case SurfaceType1:
	case SurfaceType2:
	case SurfaceType3:
	case SurfaceType4:
	default:
		ProjectileMovement->Bounciness = 0.6f;
		ProjectileMovement->Friction = 0.2f;
		break;
	}
}

