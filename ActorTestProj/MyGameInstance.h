// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StaticData.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTORTESTPROJ_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual FStaticData GetStaticData();

private:

	UMyGameInstance();

};
