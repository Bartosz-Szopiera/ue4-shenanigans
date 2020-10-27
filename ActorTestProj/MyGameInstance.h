// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTORTESTPROJ_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() {
		UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Initializing GameInstance"));
	}

private:

	UMyGameInstance();

	virtual void FSetStaticData();
};
