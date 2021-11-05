// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "LightProject/Public/LightProjectWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Float16.h"
#include "Math/Float32.h"
#include <math.h>
#include "LightProject/LightProjectGameMode.h"
#include <LightProject//Public/LightProjectRifle.h>
#include <LightProject//Public/LightProjectLauncher.h>
//////////////////////////////////////////////////////////////////////////
// ALightProjectCharacter

ALightProjectCharacter::ALightProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	// set our turn rates for input
	BaseTurnRate = 35.f;
	BaseLookUpRate = 35.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	Health = MaxHealth;


}




//////////////////////////////////////////////////////////////////////////
// Input

void ALightProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ALightProjectCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ALightProjectCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &ALightProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALightProjectCharacter::MoveRight);

	

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ALightProjectCharacter::MouseTurn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALightProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALightProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALightProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALightProjectCharacter::TouchStopped);
	PlayerInputComponent->BindTouch(IE_Repeat, this, &ALightProjectCharacter::TouchRepeated);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALightProjectCharacter::OnResetVR);
}

void ALightProjectCharacter::MouseTurn(float Val)
{
	APawn::AddControllerYawInput(Val);
	FRotator ControlRotation = GetControlRotation();
	FRotator ActorRotation = GetActorRotation();
	if (abs(ControlRotation.Yaw - ActorRotation.Yaw) > 89.0f)
	{
		SetActorRotation(UKismetMathLibrary::RInterpTo(FRotator(0, ActorRotation.Yaw, 0), FRotator(0, ControlRotation.Yaw, 0), 0.01f, 5.0f));
	}
	if (ACharacter::bIsCrouched)
	{
		SetActorRotation(UKismetMathLibrary::RInterpTo(FRotator(0, ActorRotation.Yaw, 0), FRotator(0, ControlRotation.Yaw, 0), 0.01f, 10.0f));
	}
}

float ALightProjectCharacter::GetHealthToPercent() const
{
	return Health/MaxHealth;
}

float ALightProjectCharacter::GetHealthToFloat() const
{
	return Health;
}

void ALightProjectCharacter::CreateDefaultShootWeapon()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.Instigator = this;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Weapon = GetWorld()->SpawnActor<ALightProjectWeapon>(ActorSpawnParams);
	FAttachmentTransformRules ActorAttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToActor(this, ActorAttachRules,FName(TEXT("WeaponSocket")));
}

void ALightProjectCharacter::ChangeHealth(float value)
{
	if (!bIsGameOver)
	{
		Health = FMath::Clamp(Health + value, 0.0f, MaxHealth);
		if (Health < 0.01f)
		{
			ALightProjectGameMode* GameMode = Cast<ALightProjectGameMode>(GetWorld()->GetAuthGameMode());
			OnChangerCharacterHealth();
			if (GameMode)
			{
				GameMode->PlayerOver(UGameplayStatics::GetPlayerPawn(this, 0));
			}
		}
	}
}

void ALightProjectCharacter::ChangePlayerTotalDamage(int value)
{
	PlayerTotalDamage += value;
}


void ALightProjectCharacter::OnResetVR()
{
	// If LightProject is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in LightProject.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALightProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	bIsStartTouch = true;
	FFirstTouchPoint = Location;
}

void ALightProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bIsStartTouch)
	{
		bIsStartTouch = false;
	}
}

void ALightProjectCharacter::TouchRepeated(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bIsStartTouch)
	{
		FVector VectorDelta = (Location - FFirstTouchPoint).GetSafeNormal();
		float DeltaSeconds = GetWorld()->GetDeltaSeconds();
		AddControllerYawInput(VectorDelta.X * BaseTurnRate * DeltaSeconds);
		AddControllerPitchInput(VectorDelta.Y * BaseTurnRate * DeltaSeconds);

		FFirstTouchPoint = Location;
	}
}

void ALightProjectCharacter::StartCrouch()
{
	Crouch();
}

void ALightProjectCharacter::EndCrouch()
{
	UnCrouch();
}





FVector ALightProjectCharacter::GetPawnViewLocation() const
{
	if (FollowCamera)
	{
		return FollowCamera->GetComponentLocation();
	}
	return FVector();
}

void ALightProjectCharacter::CheckGroundObjects()
{
	FVector EyeLocation;
	FRotator EyeRotaiton;
	FVector TraceEnd;
	TArray<FHitResult> HitInfo;
	UWorld* World= GetWorld();
	GetActorEyesViewPoint(EyeLocation, EyeRotaiton);
	TraceEnd = EyeLocation + (EyeRotaiton.Vector() * DetectObjectDistance);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;
	
	if (World->LineTraceMultiByChannel(HitInfo, EyeLocation, TraceEnd,ECollisionChannel::ECC_Visibility, QueryParams))
	{

		int8 HitInfoSize = HitInfo.Num();
		bool Result = false;
		for (int8 i = 0; i < HitInfoSize; i++)
		{
			OnCheckGroundObjects(HitInfo[i], Result);
			if(Result)
			{
				break;
			}
		}
	}
}

void ALightProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALightProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALightProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALightProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
