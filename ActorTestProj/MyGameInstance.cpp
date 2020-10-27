// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StaticData.h"

/**
 * Initializing static data
 */
FStaticData FSDManager::StaticData;

UMyGameInstance::UMyGameInstance() {
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] About to set static data"));
	FSetStaticData();
};

//void UMyGameInstance::Init() {
//	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Initializing GameInstance"));
//}

void UMyGameInstance::FSetStaticData() {
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Running FSetStaticData"));
	// type1
	FTypeData<ESDTypes::type1> entity1type1;
	entity1type1.id = 1;
	entity1type1.prop1 = 11;
	entity1type1.prop2 = 12;

	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));

	FSDManager::StaticData.type1.Add(entity1type1.id, entity1type1);

	// type2
	FTypeData<ESDTypes::type2> entity1type2;
	entity1type1.id = 21;
	entity1type1.prop1 = 21;
	entity1type1.prop2 = 22;

	FSDManager::StaticData.type2.Add(entity1type2.id, entity1type2);

	// Wrapping up
	FSDManager::StaticData.dataIsSet = true;
	//return FSDManager::StaticData;
}
