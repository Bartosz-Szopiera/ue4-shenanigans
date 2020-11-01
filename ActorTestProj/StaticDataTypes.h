
#pragma once

#include "Types.gen.h"

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

template<FSD::Types E>
struct FSDTypeData {
public:
	friend int32 GetTypeHash(const FSDTypeData<E>& myStruct) {
		return myStruct.id;
	};

	friend bool operator==(const FSDTypeData<E>& LHS, const FSDTypeData<E>& RHS)
	{
		return LHS.id == RHS.id;
	};
};
template<> struct FSDTypeData<FSD::Types::type1> : public FSDTypeBase, public FSD::FType1Data {};
template<> struct FSDTypeData<FSD::Types::type2> : public FSDTypeBase, public FSD::FType2Data {};

//USTRUCT(BlueprintType)
struct FStaticData {
	//GENERATED_BODY()

	TMap<int32, FSDTypeData<FSD::Types::type1>> type1;
	TMap<int32, FSDTypeData<FSD::Types::type2>> type2;

	bool dataIsReady;
};
