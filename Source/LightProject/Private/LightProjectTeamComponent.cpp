// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectTeamComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
ULightProjectTeamComponent::ULightProjectTeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void ULightProjectTeamComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams SharedParams;
	SharedParams.Condition = ELifetimeCondition::COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ULightProjectTeamComponent, TeamNum, SharedParams);
}

// Called when the game starts
void ULightProjectTeamComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULightProjectTeamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool ULightProjectTeamComponent::IsFriendlyTeam(AActor* ActorA, AActor* ActorB)
{
	if (ActorA==nullptr||ActorB==nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ULightProjectTeamComponent:At Least One Actor is Null"));
		return false;
	}
	ULightProjectTeamComponent* TeamComponentA = Cast<ULightProjectTeamComponent>(ActorA->GetComponentByClass(ULightProjectTeamComponent::StaticClass()));
	ULightProjectTeamComponent* TeamComponentB = Cast<ULightProjectTeamComponent>(ActorB->GetComponentByClass(ULightProjectTeamComponent::StaticClass()));
	if (TeamComponentA == nullptr || TeamComponentB == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ULightProjectTeamComponent:At Least One TeamComponent is Null"));
		return false;
	}
	if (TeamComponentA->TeamNum != TeamComponentB->TeamNum)
	{

		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("ULightProjectTeamComponent:TeamNums Are Equal"));
	return true;
}

