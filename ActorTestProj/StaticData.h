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

struct FType1Data: public FBasicStruct {
	uint32 id;

	int32 prop1;
	int32 prop2;
};

//struct FType2Data {
//	uint32 id;
//	uint32 GetTypeHash() { return id; }
//
//	int32 prop1;
//	int32 prop2;
//};

enum class ESDTypes {
	type1,
	type2,
};

template<ESDTypes E>
struct FTypeData {};
template<>
struct FTypeData<ESDTypes::type1> : FType1Data
{};

struct FStaticData {
	TSet<FType1Data> type1;
	//TSet<FType2Data> type2;
	bool dataIsSet;
};
