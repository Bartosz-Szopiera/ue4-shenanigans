#pragma once
#include "StaticDataCalls.h"
class FSDJuncture {
public:
	static void FSDSpecializationJuncture(int typeCode, ESDSpecializations spec) {
		auto test = [=](FSD::Types e) { return typeCode == static_cast<int>(e); };
		if (test(FSD::Types::type1))		STDCalls::SpecializedCall<FSD::Types::type1>(spec);
		else if (test(FSD::Types::type2))		STDCalls::SpecializedCall<FSD::Types::type2>(spec);
	};
};
