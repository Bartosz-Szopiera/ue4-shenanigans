// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StaticData.h"

/**
 * Initializing static data
 */
FStaticData FSDManager::StaticData;

std::string FSDManager::SDCurrentReadLine;

std::ofstream FSDManager::SDCurrentSaveFile;

UMyGameInstance::UMyGameInstance() {
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] About to set static data"));

	FSDManager::FSetStaticData();
};

//FStaticData UMyGameInstance::GetStaticData() {
//	return FSDManager::StaticData;
//}

