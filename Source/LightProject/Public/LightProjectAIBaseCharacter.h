// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LightProjectAIBaseCharacter.generated.h"

//AI״̬��ö��
UENUM(BlueprintType)
enum class EAIState :uint8
{
	Idle,
	Patrol,
	MeleeAttack,
	RangeAttack,
	Approach,
	Flee,
	StrafeAround,
	Hit

};


UCLASS()
class LIGHTPROJECT_API ALightProjectAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALightProjectAIBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
