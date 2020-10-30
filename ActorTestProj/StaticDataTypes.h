
#pragma once

//#include "StaticDataTypes.generated.h"

enum class ESDInstanceAction {
	writeToString,
	writeToInstance,
};

enum class ESDSpecializations {
	createStaticData,
	saveStaticData,
};

enum class ESDValueTypes {
	int32 = 0,
	flt,
	string,
	boolean,
};

enum class ESDTypes {
	type1 = 0,
	type2,
	type3,
	type4,
};

struct FType1Data {
	int32 id;
	int32 prop1;
	int32 prop2;
};

struct FType2Data {
	int32 id;
	int32 prop1;
	float prop2;
	FString prop3;
	bool prop4;
	TArray<int32> prop5;
	TArray<float> prop6;
	TArray<FString> prop7;
	TArray<bool> prop8;
};

struct FSDInstanceProp {
	std::string propName;
	ESDValueTypes propValueType;
	std::vector<std::string> propValues;
	bool isArray = false;
};

template<ESDTypes E>
struct FSDTypeData {
	int32 id;

	friend int32 GetTypeHash(const FSDTypeData<E>& myStruct) {
		return myStruct.id;
	};

	friend bool operator==(const FSDTypeData<E>& LHS, const FSDTypeData<E>& RHS)
	{
		return LHS.id == RHS.id;
	};
};
template<> struct FSDTypeData<ESDTypes::type1> : public FType1Data {};
template<> struct FSDTypeData<ESDTypes::type2> : public FType2Data {};

//USTRUCT(BlueprintType)
struct FStaticData {
	//GENERATED_BODY()

	TMap<int32, FSDTypeData<ESDTypes::type1>> type1;
	TMap<int32, FSDTypeData<ESDTypes::type2>> type2;

	bool dataIsSet;
};
