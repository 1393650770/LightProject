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
	//尝试开始建造物体的接口
	virtual bool TryStartBuildObject_API(FDataTableRowHandle& BuildObjectDataTableHandle) = 0;

	//设置建造模式的接口
	virtual bool SetBuidingMode_API() = 0;

	//尝试旋转的接口
	virtual bool TryRotate_API() = 0;

	//尝试更新建筑体的接口
	virtual bool TryUpgrade_API() = 0;

	//尝试移除建筑体的接口
	virtual bool TryRemove_API() = 0;

	//尝试破坏建筑体的接口
	virtual bool TryDestruct_API()=0;
};
