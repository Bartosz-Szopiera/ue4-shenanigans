#pragma once
#include "StaticData.h"
class FSDSGetTypeData {
public:
	template<FSD::Types E>
	static auto GetTypeData() { return FSDStore::StaticData.type1; };
	template<> static auto GetTypeData<FSD::Types::type1>() { return FSDStore::StaticData.type1; };
	template<> static auto GetTypeData<FSD::Types::type2>() { return FSDStore::StaticData.type2; };
};