// 2020 Purple Pill Productions.

#pragma once

struct FBasicStruct {
	uint32 id;

	friend uint32 GetTypeHash(const FBasicStruct& myStruct) {
		return myStruct.id;
	}

	friend bool operator==(const FBasicStruct& LHS, const FBasicStruct& RHS)
	{
		return LHS.id == RHS.id;
	}
};

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

class FSDManager {
	FSDManager() {
		//initStaticData();
	};

private:
	/*void initStaticData() {
		if (!StaticData.dataIsSet) {
			FSetStaticData();
		}
	};*/

public:
	// Does it need to be static after all?
	static FStaticData StaticData;

	//template<ESDTypes E>
	//static TMap<uint32, FTypeData<E>> getTypeData() {
	//	switch (E)
	//	{
	//	case ESDTypes::type1:
	//		return StaticData.type1;
	//		//break;
	//	case ESDTypes::type2:
	//		return StaticData.type2;
	//		//break;
	//	}
	//};
	static TMap<uint32, FTypeData<ESDTypes::type1>> getTypeData() {
		return StaticData.type1;
		//switch (E)
		//{
		//case ESDTypes::type1:
		//	return StaticData.type1;
		//	//break;
		//case ESDTypes::type2:
		//	return StaticData.type2;
		//	//break;
		//}
	};

	//template<ESDTypes E>
	//static FTypeData<E>& getTypeInstanceData(ESDTypes dataType, uint32 instanceId) {
	static FTypeData<ESDTypes::type1>& getTypeInstanceData(uint32 instanceId) {
		//TMap<uint32, FTypeData<E>> typeData = getTypeData<E>(dataType);
		//TMap<uint32, FTypeData<E>> typeData = getTypeData<E>();
		TMap<uint32, FTypeData<ESDTypes::type1>> typeData = getTypeData();
		bool hasInstance = typeData.Contains(instanceId);
		if (!hasInstance) {
			UE_LOG(LogTemp, Warning, TEXT("[MYLOG] There was no item"));
		}
		return typeData[instanceId];
	};

	static bool staticDataLoaded() {
		return StaticData.dataIsSet;
	};
};
