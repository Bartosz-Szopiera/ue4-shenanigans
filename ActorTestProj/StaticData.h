// 2020 Purple Pill Productions.

#pragma once

#include "StaticData.generated.h"

enum class EValueTypes {
	uint32 = 0 ,
	int32,
	flt,
	string,
};

//enum class EComplexValueTypes {
//	arr
//};
//
//struct FSDSimple {
//	ESimpleValueTypes valueType,
//	string value,
//};
//
//struct FSDComplex {
//	EComplexValueTypes valueType,
//	FSDSimple value[],
//};
//
//union UValueType {
//	FSDSimple simple,
//	FSDComplex complex,
//};

struct FPropValue {
	EValueTypes valueType;
	string value[];
};

struct FPropDescriptor {
	string propName;
	FPropValue propValue;
};

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

struct extractedChunks {
	string chunks[];
	uint32 count;
};

extractedChunks extractChunks(char delimiter = ';', std::string data) {
	uint32 chunkStart = 0;
	uint32 chunkEnd = 0;
	extractedChunks extracted;
	uint32 extracted.count = 0;
	// TODO: try taking this length down as low as possible
	string extracted.chunks[20];

	auto saveChunk = [&]() {
		uint32 length = chunkEnd - chunkStart;
		extracted.chunks[extracted.count] = data.substr(chunkStart, length);
		extracted.count++;
	}

	for (char c : encodedInstance) {
		if (c == delimiter || c == '\n') {
			saveChunk();
			chunkStart = chunkEnd;
		}
		chunkEnd++;
	};

	return extracted;
};

/**
 * 
 */
void FDecodeInstanceData(std::string encodedInstance) {
	int32 typeCode = encodedInstance[0];
	encodedInstance.erase(encodedInstance.begin());
	extractedChunks instancePropsChunks = extractChunks(';', encodedInstance);

	// Array of PropDescriptors
	FPropDescriptor propValues[instancePropsChunks.count];

	string propName;
	string propValueType;
	string propValues[20];

	for (int32 i = 0; i <= instancePropsChunks.count; i++) {
		string propChunk = instancePropsChunks[i];
		extractedChunks propDescriptors = extractChunks(',', propChunk);

		propName = propDescriptors.chunks[1];
		propValueType = propDescriptors.chunks[0];

		if (propDescriptors.count == 3) { // simple data
			propValues[1] = propDescriptors.chunks[2];
		}
		else { // array
			for (string i = 0; (i + 2) < propDescriptors.chunks.size(); ++i) {
				propValues[i] = propDescriptors.chunks[i + 2];
			}
		}

		propValues[i].propName = 
	}

	auto parsePropChunk = [](std:string propChunk) {
		extractedChunks propDescriptorChunks = extractChunks(',', propChunk);
		
		if (propDescriptorChunks.count == 3) { // simple data

		}
		else { // array

		}
	}

	FSetInstance(static_cast<ESDTypes>(static_cast<int>(typeCode)), propChunks);
};


void FSetInstance(ESDTypes type, string propChunks[]) {

};

void FSetProperty(void* propPtr, string propName, std::string encodedData) {
	// Compare types and names and throw in case of mismatch.
};

/**
 * 
 */
void FEncodeInstanceData() {
	
};

void FSetInstanceOfType1() {
	FTypeData<ESDTypes::type1> instance;
	FSetProperty(&instance.prop1, "prop1", )

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
