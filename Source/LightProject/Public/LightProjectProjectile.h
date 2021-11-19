// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightProjectProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPawnNoiseEmitterComponent;
class USoundBase;
class UDamageType;
UCLASS()
class LIGHTPROJECT_API ALightProjectProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosionEffect")
	UParticleSystem* ExplosionEffect;

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	USoundBase* BrokenSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	float BrokenDamage = 180.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	float BrokenSphereRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	float LeastSpeedLimit = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	TArray<TEnumAsByte<EObjectTypeQuery> > BrokenObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	UClass* BrokenActorClassFilter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	TArray<AActor*> BrokenActorsToIgnore;
	
	TArray<AActor*> BrokenOutActors;

	TArray<UPrimitiveComponent*> BrokenOutComponents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Broken)
	float BrokenImpuseSize=100.0f;

	UFUNCTION(BlueprintCallable, Category = Projectile)
	void Broken();

	void ChangeBounciness(const FHitResult& Hit);

public:
	ALightProjectProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
