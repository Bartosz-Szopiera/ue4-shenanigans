// 2020 Purple Pill Productions.

#pragma once

#include <any>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include "Misc/DefaultValueHelper.h"
#include "StaticData.generated.h"

enum class EValueTypes {
	int32 = 0 ,
	flt,
	string,
	boolean,
};

struct FBasicStruct {
	int32 id;

	void InitializeSefl() {};

	friend int32 GetTypeHash(const FBasicStruct& myStruct) {
		return myStruct.id;
	}

	friend bool operator==(const FBasicStruct& LHS, const FBasicStruct& RHS)
	{
		return LHS.id == RHS.id;
	}
};

struct FType1Data : public FBasicStruct {
	int32 id;

	int32 prop1;
	int32 prop2;
};

struct FType2Data : public FBasicStruct {
	int32 id;

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
	int32 count;
};

extractedChunks extractChunks(char delimiter = ';', std::string data) {
	int32 chunkStart = 0;
	int32 chunkEnd = 0;
	extractedChunks extracted;
	int32 extracted.count = 0;
	// TODO: try taking this length down as low as possible
	std::vector<std:string> extracted.chunks;

	auto saveChunk = [&]() {
		int32 length = chunkEnd - chunkStart;
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

FString castStdStringToFstring(std::string value) {
	try
	{
		return FString(prop.value.c_str())
	}
	catch (Exception* e)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"s%\" to FString.", value));
		return (FString)"0conversionError";
	}
};

int32 castStdStringToInt32(std::string value) {
	try
	{
		FString fstr = castStdStringToFstring(value);
		int32 out;
		FDefaultValueHelper::ParseInt(fstr, out);
		return out;
	}
	catch (Exception* e)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"s%\" to int32.", value));
		return 0;
	}
};

float castStdStringToFloat(std::string value) {
	try
	{
		std::string::size_type sz;   // alias of size_t
		return std::stof(value, &sz);
	}
	catch (Exception* e)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"s%\" to float.", value));
		return 0;
	}
};

bool castStdStringToBool(std::string value) {
	if (value == "0") {
		return false;
	}
	else if (value == "1") {
		return true;
	}
	else if (value == "true") {
		return true;
	}
	else if (value == "false") {
		return false;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"s%\" to bool.", value));
		return false;
	}
};


void FSetProperty(std::any& instanceProperty, std::string propName, std::vector<Prop>& properties) {
	bool found = false;

	auto GetValue = [](Prop& prop, int32 i = 0) {
		switch (prop.propValueType)
		{
		case EValueTypes::int32:
			return castStdStringToInt32(prop.propValues[i]);
		case EValueTypes::flt:
			return castStdStringToFloat(prop.propValues[i]);
		case EValueTypes::string:
			return castStdStringToFstring(prop.propValues[i]);
		case EValueTypes::boolean:
			return castStdStringToBool(prop.propValues[i]);
		}
	};

	for (Prop prop : properties) {
		if (propName == prop.propName) {
			found = true;
			if (prop.isArray) {
				for (int32 i = 0; i < prop.propValues.size(); i++) {
					instanceProperty[i] = GetValue(prop, i);
				}
			}
			else {
				instanceProperty = GetValue(prop, 0);
			}
		}
	}
	if (!found) {
		UE_LOG(LogTemp, Warning, TEXT("---------> !!! Prop of that name not found"));
		// :(
	}
};

template<ESDTypes E>
void FSetInstance(FTypeData<E>& inst, std::vector<Prop>& instanceProps) {};
template<> void FSetInstance<ESDTypes::type1>(FTypeData<ESDTypes::type1>& inst, std::vector<Prop>& instanceProps) {
	FSetProperty(inst.prop1, "prop1", instanceProps);
	FSetProperty(inst.prop2, "prop2", instanceProps);
	FSetProperty(inst.id, "id", instanceProps);

	UE_LOG(LogTemp, Warning, TEXT("---------> adding static data to map..."));
	StaticData.type1.Add(inst.id, inst);
};
template<> void FSetInstance<ESDTypes::type2>(FTypeData<ESDTypes::type2>& inst, std::vector<Prop>& instanceProps) {
	FSetProperty(inst.prop1, "prop1", instanceProps);
	FSetProperty(inst.prop2, "prop2", instanceProps);
	FSetProperty(inst.id, "id", instanceProps);

	UE_LOG(LogTemp, Warning, TEXT("---------> adding static data to map..."));
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
		TMap<int32, FTypeData<currentType>> typeData = FSDManager::getTypeData<currentType>();

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
	/*int32 id;

	void InitializeSefl() {};

	friend int32 GetTypeHash(const FBasicStruct& myStruct) {
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

	TMap<int32, FTypeData<ESDTypes::type1>> type1;
	TMap<int32, FTypeData<ESDTypes::type2>> type2;

	bool dataIsSet;
};

class FSDManager {
public:
	static FStaticData StaticData;

	static bool staticDataLoaded() {
		return StaticData.dataIsSet;
	};

	template<ESDTypes E>
	static FTypeData<E>& getTypeInstanceData(int32 instanceId) {
		TMap<int32, FTypeData<E>> typeData = getTypeData<E>();
		bool hasInstance = typeData.Contains(instanceId);
		if (!hasInstance) {
			UE_LOG(LogTemp, Warning, TEXT("---------> There was no item"));
		}
		return typeData[instanceId];
	};

	template<ESDTypes E>
	static TMap<int32, FTypeData<E>> getTypeData() { return StaticData.type1; };
	template<> static TMap<int32, FTypeData<ESDTypes::type1>> getTypeData<ESDTypes::type1>() { return StaticData.type1; };
	template<> static TMap<int32, FTypeData<ESDTypes::type2>> getTypeData<ESDTypes::type2>() { return StaticData.type2; };

	static void FSetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Running FSetStaticData"));

		FParseRawStaticData();

		// type1
		//FTypeData<ESDTypes::type1> entity1type1;
		//entity1type1.id = 1;
		//entity1type1.prop1 = 11;
		//entity1type1.prop2 = 12;

		//UE_LOG(LogTemp, Warning, TEXT("---------> adding static data to map..."));

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
