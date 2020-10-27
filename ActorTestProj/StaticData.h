// 2020 Purple Pill Productions.

#pragma once

//#include "CoreMinimal.h"
//#include "Math/NumericLimits.h"
//#include "Containers/Set.h"

struct FBasicStruct {
	uint32 id;

	friend uint32 GetTypeHash();

	friend bool operator==(const FBasicStruct& LHS, const FBasicStruct& RHS);
};

bool operator==(const FBasicStruct& LHS, const FBasicStruct& RHS)
{
	return LHS.id == RHS.id;
}

uint32 GetTypeHash(const FBasicStruct& myStruct) {
	return myStruct.id;
}

struct FType1Data : public FBasicStruct {
	uint32 id;

	int32 prop1;
	int32 prop2;
};

struct FType2Data : public FBasicStruct {
	uint32 id;

	int32 prop1;
	int32 prop2;
};

enum class ESDTypes {
	type1,
	type2,
};

template<ESDTypes E>
struct FTypeData {};
template<> struct FTypeData<ESDTypes::type1> : public FType1Data {};
template<> struct FTypeData<ESDTypes::type2> : public FType2Data {};

struct FStaticData {
	TMap<uint32, FTypeData<ESDTypes::type1>> type1;
	TMap<uint32, FTypeData<ESDTypes::type2>> type2;

	bool dataIsSet;
};
