// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuildComponentInterface.generated.h"
#include "Engine/DataTable.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTPROJECT_API IBuildComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//���Կ�ʼ��������Ľӿ�
	virtual bool TryStartBuildObject_API(FDataTableRowHandle& BuildObjectDataTableHandle) = 0;

	//���ý���ģʽ�Ľӿ�
	virtual bool SetBuidingMode_API() = 0;

	//������ת�Ľӿ�
	virtual bool TryRotate_API() = 0;

	//���Ը��½�����Ľӿ�
	virtual bool TryUpgrade_API() = 0;

	//�����Ƴ�������Ľӿ�
	virtual bool TryRemove_API() = 0;

	//�����ƻ�������Ľӿ�
	virtual bool TryDestruct_API()=0;
};
