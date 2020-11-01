// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "StaticData.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTORTESTPROJ_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*UFUNCTION(BlueprintCallable, Category = "Static Data")
	static FStaticData GetStaticData() {
		return FSDManager::StaticData;
	}*/

	/*UFUNCTION(BlueprintCallable, Category = "Static Data")
	template<ESDTypes E>
	static FSDTypeData<E> GetTypeInstanceData(ESDTypes dataType) {
		return FSDManager::GetTypeInstanceData<dataType>(1);
	};*/
};
