// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StaticData.h"

/**
 * Initializing static data
 */
FStaticData FSDManager::StaticData;

UMyGameInstance::UMyGameInstance() {
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] About to set static data"));

	FSDManager::FSetStaticData();
};

