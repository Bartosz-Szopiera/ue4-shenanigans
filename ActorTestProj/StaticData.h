// 2020 Purple Pill Productions.

#pragma once

#include "StaticData.generated.h"

enum class ESimpleValueTypes {
	uint32,
	int32,
	flt,
	string,
	id,
};

enum class EComplexValueTypes {
	arr
};

struct FSDSimple {
	ESimpleValueTypes valueType,
	string value,
};

struct FSDComplex {
	EComplexValueTypes valueType,
	FSDSimple value[],
};

union UValueType {
	FSDSimple simple,
	FSDComplex complex,
};

struct FPropValue {
	string propName,
	UValueType propValue,
};

/**
 * propNames: [ {"propA"}, {"propB"}, {"propC"}... ]
 * values: [ {"Exmaple text", {"1.255487"}, {"15454215478"}, {"[ {"574587"}, {"78457833"} ]"} ]
 * types: [ {"string", {"flt"}, {"uint32"}, {"arr"} ]
 * 
 * 
 * 
 */

void FSetInstanceValues(FPropValue values[], FBasicStruct& instance) {

}


struct FBasicStruct {
	uint32 id;

	void InitializeSefl() {};

	friend uint32 GetTypeHash(const FBasicStruct& myStruct) {
		return myStruct.id;
	}

	friend bool operator==(const FBasicStruct& LHS, const FBasicStruct& RHS)
	{
		return LHS.id == RHS.id;
	}
};

template<ESDTypes E>
struct FSDSetter {};
template<> struct FTypeData<ESDTypes::type1> {};
template<> struct FTypeData<ESDTypes::type2> {};

struct FType1Data : public FBasicStruct {
	string type[1] = { "type1" };
	string values[][3] = {{"0", "0", }}

	uint32 id;

	int32 prop1;
	int32 prop2;
};

struct FType2Data : public FBasicStruct {
	uint32 id;

	int32 prop1;
	int32 prop2;
};

void FGetRawStaticData() {
	/**
	 * Reads static data file line by line
	 * 
	 */
}

void FSetEntityData() {

}

template<ESDTypes E>
void FSetEntityData(FTypeData<E>& e) { e.prop1; };
template<> void FSetEntityData<ESDTypes::type1>() { return StaticData.type1; };
template<> void FSetEntityData<ESDTypes::type2>() { return StaticData.type2; };

void FSetProperty(void* propPtr, string propName, string encodedData) {
	// Compare types and names and throw in case of mismatch.
};

/**
 * 
 */
void FDecodeInstanceData(std::string encodedInstance) {
	char typeCode;
	char delimiter = ';';
	string propChunks[20]; // assuming that there is no more than 20 props

	uint32 chunkStart = 0;
	uint32 chunkEnd = 0;
	uint32 propCount = 0;
	
	auto copyCurrentChunk = []() {
		uint32 length = chunkEnd - chunkStart;
		char temp[length];
		encodedInstance.copy((char*)temp, length, chunkStart);
		return temp;
	};

	auto saveProp = [&]() { propChunks[propCount] = copyCurrentChunk(); }
	auto saveType = [&]() { typeCode = copyCurrentChunk(); }

	for (char c : encodedInstance) {
		if (c == delimiter) {
			if (chunkStart == 0) { saveType(); } // type signature
			else { saveProp(); propCount++; } // prop signature
			chunkStart = chunkEnd;
		}
		else if (c == '\n') { saveProp() } // last prop signature
		chunkEnd++;
	};
};

/**
 * 
 */
void FEncodeInstanceData() {
	
};

void FSetInstanceOfType1() {
	FTypeData<ESDTypes::type1> instance;
	FSetProperty(instance.prop1, "prop1", )

	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));
	StaticData.type1.Add(instance.id, instance);
}

void FInitializeStaticData() {

	switch (ESDTypes)
	{
	case ESDTypes::type1:
		break;
	case ESDTypes::type2:

	}
	FTypeData<ESDTypes::type1>

	FTypeData<ESDTypes::type1> entity1type1;
	entity1type1.id = 1;
	entity1type1.prop1 = 11;
	entity1type1.prop2 = 12;

	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));

	StaticData.type1.Add(entity1type1.id, entity1type1);

};

enum class ESDTypes {
	type1 = 0,
	type2 = 1,
};

template<ESDTypes E>
struct FTypeData {
	/*uint32 id;

	void InitializeSefl() {};

	friend uint32 GetTypeHash(const FBasicStruct& myStruct) {
		return myStruct.id;
	}

	friend bool operator==(const FBasicStruct& LHS, const FBasicStruct& RHS)
	{
		return LHS.id == RHS.id;
	}*/
};
template<> struct FTypeData<ESDTypes::type1> : public FType1Data {};
template<> struct FTypeData<ESDTypes::type2> : public FType2Data {};

USTRUCT(BlueprintType)
struct FStaticData {
	GENERATED_BODY()

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
	static FTypeData<E>& getTypeInstanceData(uint32 instanceId) {
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
