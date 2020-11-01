#pragma once

namespace FSD {
	enum class ValueTypes {
		int32 = 0,
		flt,
		string,
		boolean,
	};

	enum class Types {
		type1 = 0,
		type2,
		type3,
	};

	struct FType1Data {
		int32 prop1;
		int32 prop2;
	};

	struct FType2Data {
		int32 prop1;
		float prop2;
		FString prop3;
		bool prop4;
		TArray<int32> prop5;
		TArray<float> prop6;
		TArray<FString> prop7;
		TArray<bool> prop8;
	};
};
