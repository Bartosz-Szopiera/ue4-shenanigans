
#pragma once

#include "TypesExtended.gen.h"
#include "SDMainType.gen.h"

struct FSDInstanceProp {
	std::string propName;
	FSD::ValueTypes propValueType;
	std::vector<std::string> propValues;
	bool isArray = false;
};

enum class ESDInstanceAction {
	savingStaticData,
	loadingStaticData,
};

enum class ESDSpecializations {
	createStaticData,
	saveStaticData,
};
