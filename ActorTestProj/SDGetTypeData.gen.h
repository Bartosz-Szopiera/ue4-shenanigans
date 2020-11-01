#pragma once
#include "SDCore.h"
class FSDSGetTypeData {
public:
	template<FSD::Types E>
	static auto GetTypeData() { return FSDCore::StaticData.type1; };
	template<> static auto GetTypeData<FSD::Types::type1>() { return FSDCore::StaticData.type1; };
	template<> static auto GetTypeData<FSD::Types::type2>() { return FSDCore::StaticData.type2; };
};