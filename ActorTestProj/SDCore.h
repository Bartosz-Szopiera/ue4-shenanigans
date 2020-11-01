
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

	struct FSDInstanceProp {
		std::string propName;
		FSD::ValueTypes propValueType;
		std::vector<std::string> propValues;
		bool isArray = false;
	};

	struct FSDTypeBase {
		int32 id;
		bool meta_isBound;
	};

	enum class ESDInstanceAction {
		savingStaticData,
		loadingStaticData,
	};

	enum class ESDSpecializations {
		createStaticData,
		saveStaticData,
	};
};