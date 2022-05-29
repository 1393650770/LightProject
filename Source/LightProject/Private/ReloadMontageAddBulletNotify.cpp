// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadMontageAddBulletNotify.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <LightProject/LightProjectCharacter.h>
#include <LightProject/Public/LightProjectWeapon.h>

void UReloadMontageAddBulletNotify::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams SharedParams;
	SharedParams.Condition = ELifetimeCondition::COND_None;

}

void UReloadMontageAddBulletNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UReloadMontageAddBulletNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}

void UReloadMontageAddBulletNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Player = MeshComp->GetOwner();
	if (Player)
	{
		ALightProjectCharacter* MyCharacter = Cast<ALightProjectCharacter>(Player);
		if (MyCharacter)
		{
			ALightProjectWeapon* weapon = MyCharacter->GetWeapon();
			EWeaponType weapon_type = MyCharacter->GetWeaponType();
			if (weapon)
			{
				if (weapon_type == EWeaponType::Rifle)
				{
					weapon->AddBulletNum(30);
				}
				else
				{
					weapon->AddBulletNum(1);
				}
			}
			MyCharacter->SetReloadFalseServerByCharacter();
			UE_LOG(LogTemp, Log, TEXT("Play Montage Finish!!!!!!!!!!!!"));
		}
	}
}
