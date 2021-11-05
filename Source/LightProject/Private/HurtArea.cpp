// Fill out your copyright notice in the Description page of Project Settings.

#include "HurtArea.h"
#include "Components/BoxComponent.h"
#include "LightProject/LightProjectCharacter.h"
#include "LightProject/LightProjectGameMode.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PrimitiveComponent.h"
// Sets default values
AHurtArea::AHurtArea()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;
	OverlapComp->SetHiddenInGame(false);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AHurtArea::HandleOverlapStart);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AHurtArea::HandleOverlapEnd);
}

void AHurtArea::HandleOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Enter HurtArea"));
	ALightProjectCharacter* MyPawn = Cast<ALightProjectCharacter>(OtherActor);

	if (MyPawn == nullptr)
	{
		return;
	}
	GetWorldTimerManager().ClearTimer(TimerHandle_Hurt);
	FTimerDelegate HurtPlayerDelegate= FTimerDelegate::CreateUObject(this, &AHurtArea::HurtPlayer, MyPawn);
	GetWorldTimerManager().SetTimer(TimerHandle_Hurt, HurtPlayerDelegate, HurtPlayerDelayTime, true);
}



void AHurtArea::HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Exit	 HurtArea"));
	GetWorldTimerManager().ClearTimer(TimerHandle_Hurt);
}

void AHurtArea::SetPlayerState(EPlayerState NewState)
{

	PlayerState = NewState;

	OnStateChanged(PlayerState);
}


void AHurtArea::HurtPlayer(ALightProjectCharacter* MyPawn)
{
	float hurtNum = -(rand() % 5+1);
	if (MyPawn->GetHealthToFloat() >= 0.01f)
	{
		UGameplayStatics::ApplyDamage(MyPawn, hurtNum, nullptr, this, DamageTypeClass);
	}
	//MyPawn->ChangeHealth(hurtNum);
	UE_LOG(LogTemp, Log, TEXT("%f"), hurtNum);
}
