#pragma once

#include "StaticDataTypes.generated.h"

enum class EValueTypes {
	int32 = 0,
	flt,
	string,
	boolean,
};

enum class ESDTypes {
	type1 = 0,
	type2 = 1,
};

enum class EInstanceAction {
	writeToString,
	writeToInstance,
};

struct FType1Data {
	int32 id;
	int32 prop1;
	int32 prop2;
};

struct FType2Data {
	int32 id;
	int32 prop1;
	int32 prop2;
};

struct FSDInstanceProp {
	std::string propName;
	EValueTypes propValueType;
	std::vector<std::string> propValues;
	bool isArray = false;
};

template<ESDTypes E>
struct FTypeData {
	int32 id;

	friend int32 GetTypeHash(const FTypeData<E>& myStruct) {
		return myStruct.id;
	};

	friend bool operator==(const FTypeData<E>& LHS, const FTypeData<E>& RHS)
	{
		return LHS.id == RHS.id;
	};
};
template<> struct FTypeData<ESDTypes::type1> : public FType1Data {};
template<> struct FTypeData<ESDTypes::type2> : public FType2Data {};

USTRUCT(BlueprintType)
struct FStaticData {
	GENERATED_BODY()

	TMap<int32, FTypeData<ESDTypes::type1>> type1;
	TMap<int32, FTypeData<ESDTypes::type2>> type2;

	bool dataIsSet;
};
