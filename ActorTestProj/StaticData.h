// 2020 Purple Pill Productions.

#pragma once

#include <any>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include "StaticData.generated.h"

enum class EValueTypes {
	uint32 = 0 ,
	int32,
	flt,
	string,
	boolean,
};

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

void FParseRawStaticData() {
	std::ifstream infile("thefile.txt");
	std::string line;

	while (std::getline(infile, line))
	{
		FDecodeInstanceData(line);
	}
}

struct extractedChunks {
	std::vector<std::string> chunks;
	uint32 count;
};

extractedChunks extractChunks(char delimiter = ';', std::string data) {
	uint32 chunkStart = 0;
	uint32 chunkEnd = 0;
	extractedChunks extracted;
	uint32 extracted.count = 0;
	// TODO: try taking this length down as low as possible
	std::vector<std:string> extracted.chunks;

	auto saveChunk = [&]() {
		uint32 length = chunkEnd - chunkStart;
		extracted.chunks[extracted.count] = data.substr(chunkStart, length);
		extracted.count++;
	}

	for (char c : encodedInstance) {
		if (c == delimiter) {
			saveChunk();
			chunkStart = chunkEnd;
		}
		chunkEnd++;
	};

	return extracted;
};

struct Prop {
	std::string propName;
	EValueTypes propValueType;
	std::vector<string> propValues;
	bool isArray = false;
};

/**
* 
 * Example data format:
 * 
 * 0;0,prop1Name,1347;3,prop2Name,isArray,textValue1,textValue2;\n
 * 
 * ; - delimiter
 * 
 * 0 - first zero - it's numerical value of type according to ESDTypes
 * 
 * ;0,prop1Name,1347; - this is first property
 *		0			- numerical value of type of value held according to EValueTypes
 *		prop1Name	- property name
 *		1347		- value
 * 
 * ;3,prop2Name,,textValue1,textValue2; - second property which is an array
 *		3			- numerical code for the value type
 *		prop2Name	- property name
 *		isArr		- this value is not used for anything but gives one more chunk that allows
 *						to identify series of chunks as an encoding of array-holding prop
 *		textValue1	- first of series of subsequent values the array is holding
 */
void FDecodeInstanceData(std::string encodedInstance) {
	int32 typeCode = encodedInstance[0];
	encodedInstance.erase(encodedInstance.begin());
	extractedChunks instancePropsChunks = extractChunks(';', encodedInstance);

	std::vector<Prop> instanceProps;
	ESDTypes instanceType = static_cast<ESDTypes>(static_cast<int>(typeCode));
	FTypeData<instanceType> instanceStruct;

	for (int32 i = 0; i <= instancePropsChunks.count; i++) {
		Prop prop;

		string propChunk = instancePropsChunks[i];
		extractedChunks propDescriptors = extractChunks(',', propChunk);

		prop.propName = propDescriptors.chunks[1];
		prop.propValueType = static_cast<EValueTypes>(static_cast<int>(propDescriptors.chunks[0]));

		if (propDescriptors.count == 3) { // simple data
			prop.propValues[1] = propDescriptors.chunks[2];
		}
		else { // array
			prop.isArray = true;
			for (std::string i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
				prop.propValues[i] = propDescriptors.chunks[i + 3];
			}
		}

		instanceProps[i] = prop;
	}

	FSetInstance<instanceType>(instanceStruct, instanceProps);
};



void FSetProperty(std::any& instanceProperty, std::string propName, std::vector<Prop>& properties) {
	bool found = false;

	auto GetValue = [](Prop& prop, uint32 i = 0) {
		switch (prop.propValueType)
		{
		case EValueTypes::uint32:
			return static_cast<uin32>(prop.propValues[i]);
		case EValueTypes::int32:
			return static_cast<in32>(prop.propValues[i]);
		case EValueTypes::flt:
			return static_cast<float>(prop.propValues[i]);
		case EValueTypes::string:
			return FString(prop.propValues[i].c_str());
		case EValueTypes::boolean:
			return static_cast<bool>(prop.propValues[i]);
		}
	};

	for (Prop prop : properties) {
		if (propName == prop.propName) {
			found = true;
			if (prop.isArray) {
				for (uint32 i = 0; i < prop.propValues.size(); i++) {
					instanceProperty[i] = GetValue(prop, i);
				}
			}
			else {
				instanceProperty = GetValue(prop, 0);
			}
		}
	}
	if (!found) {
		UE_LOG(LogTemp, Warning, TEXT("[MYLOG] !!! Prop of that name not found"));
		// :(
	}
};

template<ESDTypes E>
void FSetInstance(FTypeData<E>& inst, std::vector<Prop>& instanceProps) {};
template<> void FSetInstance<ESDTypes::type1>(FTypeData<ESDTypes::type1>& inst, std::vector<Prop>& instanceProps) {
	FSetProperty(inst.prop1, "prop1", instanceProps);
	FSetProperty(inst.prop2, "prop2", instanceProps);
	FSetProperty(inst.id, "id", instanceProps);

	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));
	StaticData.type1.Add(inst.id, inst);
};
template<> void FSetInstance<ESDTypes::type2>(FTypeData<ESDTypes::type2>& inst, std::vector<Prop>& instanceProps) {
	FSetProperty(inst.prop1, "prop1", instanceProps);
	FSetProperty(inst.prop2, "prop2", instanceProps);
	FSetProperty(inst.id, "id", instanceProps);

	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));
	StaticData.type1.Add(inst.id, inst);
};

/**
 * 
 */
void FEncodeInstanceData() {
	
};

void SaveStaticData() {
	std::ofstream myfile;
	myfile.open("example.txt");
	string line
	for (int i = ESDTypes::type1; i != ESDTypes::type2; i++)
	{
		ESDTypes currentType = static_cast<ESDTypes>(static_cast<int>(typeCode));
		TMap<uint32, FTypeData<currentType>> typeData = FSDManager::getTypeData<currentType>();

		for () // for each instance
		{
			line = FEncodeInstanceData()
		}
	}
	myfile << line << endl;
	myfile.close();
}

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

		FParseRawStaticData();

		// type1
		//FTypeData<ESDTypes::type1> entity1type1;
		//entity1type1.id = 1;
		//entity1type1.prop1 = 11;
		//entity1type1.prop2 = 12;

		//UE_LOG(LogTemp, Warning, TEXT("[MYLOG] adding static data to map..."));

		//StaticData.type1.Add(entity1type1.id, entity1type1);

		//// type2
		//FTypeData<ESDTypes::type2> entity1type2;
		//entity1type1.id = 21;
		//entity1type1.prop1 = 21;
		//entity1type1.prop2 = 22;

		//StaticData.type2.Add(entity1type2.id, entity1type2);

		//// Wrapping up
		//StaticData.dataIsSet = true;
		////return FSDManager::StaticData;
	}
};
