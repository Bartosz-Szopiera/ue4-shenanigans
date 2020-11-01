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
		return FSDCore::StaticData;
	}*/

	/*UFUNCTION(BlueprintCallable, Category = "Static Data")
	template<Types E>
	static FSDTypeData<E> GetTypeInstanceData(Types dataType) {
		return FSDCore::GetTypeInstanceData<dataType>(1);
	};*/
};
