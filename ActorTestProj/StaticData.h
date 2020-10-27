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
public:
	static FStaticData StaticData;

	static bool staticDataLoaded() {
		return StaticData.dataIsSet;
	};

	template<ESDTypes E>
	static FTypeData<ESDTypes::type1>& getTypeInstanceData(uint32 instanceId) {
		TMap<uint32, FTypeData<E>> typeData = getTypeData<E>();
		bool hasInstance = typeData.Contains(instanceId);
		if (!hasInstance) {
			UE_LOG(LogTemp, Warning, TEXT("[MYLOG] There was no item"));
		}
		return typeData[instanceId];
	};

	template<ESDTypes E>
	static TMap<uint32, FTypeData<E>> getTypeData() { return StaticData.type1; };
	template<> static TMap<uint32, FTypeData<ESDTypes::type1>> getTypeData<ESDTypes::type1>() { return StaticData.type1; };
	template<> static TMap<uint32, FTypeData<ESDTypes::type2>> getTypeData<ESDTypes::type2>() { return StaticData.type2; };

	static void FSetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Running FSetStaticData"));
		// type1
		FTypeData<ESDTypes::type1> entity1type1;
		entity1type1.id = 1;
		entity1type1.prop1 = 11;
		entity1type1.prop2 = 12;

		UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));

		StaticData.type1.Add(entity1type1.id, entity1type1);

		// type2
		FTypeData<ESDTypes::type2> entity1type2;
		entity1type1.id = 21;
		entity1type1.prop1 = 21;
		entity1type1.prop2 = 22;

		StaticData.type2.Add(entity1type2.id, entity1type2);

		// Wrapping up
		StaticData.dataIsSet = true;
		//return FSDManager::StaticData;
	}
};
