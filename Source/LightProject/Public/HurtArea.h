// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HurtArea.generated.h"

class UBoxComponent;
class UParticleSystem;
class UPrimitiveComponent;
class ALightProjectCharacter;
UENUM(BlueprintType)
enum class EPlayerState :uint8
{
	Normal,
	Hurt,
};

UCLASS()
class LIGHTPROJECT_API AHurtArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHurtArea();

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "FireArea")
	int CurrentInAreaPeople=0;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Player)
	float HurtPlayerDelayTime=0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Player)
	FTimerHandle TimerHandle_Hurt;

	EPlayerState PlayerState = EPlayerState::Normal;

	UFUNCTION()
	void HandleOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void SetPlayerState(EPlayerState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void OnStateChanged(EPlayerState NewState);

	UFUNCTION(Server,Reliable)
	void HurtPlayer(ALightProjectCharacter* MyPawn);

public:	

};
