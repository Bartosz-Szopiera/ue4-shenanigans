
#pragma once

#include <vector>
#include <string>
#include "SDTypes.h"
#include "SDUtil.h"

class FSDCore {
public:
	static FStaticData StaticData;

	static FString CurrentInstanceString;

	static FString StaticDataFString;

	static bool StaticDataIsReady() {
		FString text = StaticData.dataIsReady ? "YES" : "NO";
		UE_LOG(LogTemp, Warning, TEXT("---------> Is data set?: %s"), *text);
		return StaticData.dataIsReady;
	};
};