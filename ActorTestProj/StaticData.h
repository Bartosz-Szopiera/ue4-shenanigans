// 2020 Purple Pill Productions.

#pragma once

#include <any>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "StaticDataTypes.h"
#include "StaticDataHelp.h"

/**
*
 * Data formating example:
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
 *
 */

std::string currentReadLine;
std::ofstream currentSaveFile;

void FSetValueFromString(std::string source, int32& target) { target = castStdStringToInt32(source); }
void FSetValueFromString(std::string source, float& target) { target = castStdStringToFloat(source); }
void FSetValueFromString(std::string source, FString& target) { target = castStdStringToFstring(source); }
void FSetValueFromString(std::string source, bool& target) { target = castStdStringToBool(source); }

template<class T>
void FSDPropertyAction(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, EInstanceAction action) {
//void FSDPropertyAction(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, EInstanceAction action) {
	auto encodeValueInString = []() {
		if (std::is_same<decltype(source), int32>::value) {
			prop.propValueType = EValueTypes::int32;
			fstr = FString::FromInt(source);
			out = std::string(TCHAR_TO_UTF8(*fstr));
		}
		else if (std::is_same<decltype(source), float>::value) {
			prop.propValueType = EValueTypes::flt;
			fstr = FString::SanitizeFloat(source);
			out = std::string(TCHAR_TO_UTF8(*fstr));
		}
		else if (std::is_same<decltype(source), FString>::value) {
			prop.propValueType = EValueTypes::string;
			out = std::string(TCHAR_TO_UTF8(*source));
		}
		else if (std::is_same<decltype(source), bool>::value) {
			prop.propValueType = EValueTypes::boolean;
			out = source ? "0" : "1";
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Instance property type not matching known value types."));
		}
		return out
	};
	if (action == EInstanceAction::toInstance) {
		bool found = false;

		for (FSDInstanceProp prop : properties) {
			if (propName == prop.propName) {
				found = true;
				if (prop.isArray) {
					for (int32 i = 0; i < prop.propValues.size(); i++) {
						SetValueFromString(prop, instanceProperty[i], i);
					}
				}
				else {
					instanceProperty = GetValue(prop, 0);
				}
			}
		}
		if (!found) {
			UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Prop of name not found."));
			// :(
		}
	}
	else {
		FSDInstanceProp prop;
		prop.propName = propName;

		auto encodeValueInString = [](auto& source) {
			std:string out;
			FString fstr;
			if (std::is_same<decltype(source), int32>::value) {
				prop.propValueType = EValueTypes::int32;
				fstr = FString::FromInt(source);
				out = std::string(TCHAR_TO_UTF8(*fstr));
			}
			else if (std::is_same<decltype(source), float>::value) {
				prop.propValueType = EValueTypes::flt;
				fstr = FString::SanitizeFloat(source);
				out = std::string(TCHAR_TO_UTF8(*fstr));
			}
			else if (std::is_same<decltype(source), FString>::value) {
				prop.propValueType = EValueTypes::string;
				out = std::string(TCHAR_TO_UTF8(*source));
			}
			else if (std::is_same<decltype(source), bool>::value) {
				prop.propValueType = EValueTypes::boolean;
				out = source ? "0" : "1";
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Instance property type not matching known value types."));
			}
			return out
		};

		if (std::is_same<decltype(instanceProperty), TArray>::value) {
			prop.isArray = true;
			for (auto& value : instanceProperty)
			{
				prop.propValues.push_back(encodeValueInString(value));
			}
		}
		else {
			prop.isArray = false;
			prop.propValues.push_back(encodeValueInString(instanceProperty));
		};

		properties.push_back(prop);
	};
};

enum class ESDSpecializations {
	createStaticData,
	saveStaticData,
};

std::vector<FSDInstanceProp> FDecodeInstanceData() {
	std::string encodedInstance = currentReadLine;
	encodedInstance.erase(encodedInstance.begin());
	extractedChunks instancePropsChunks = extractChunks(encodedInstance);

	std::vector<FSDInstanceProp> instanceProps;

	// Parsing prop by prop
	for (std::string propChunk : instancePropsChunks.chunks) {
		FSDInstanceProp prop;

		extractedChunks propDescriptors = extractChunks(propChunk, ',');

		//prop.propValueType = static_cast<EValueTypes>(static_cast<int>(propDescriptors.chunks[0]));
		int32 typeCode = castStdStringToInt32(propDescriptors.chunks[0]);
		prop.propValueType = static_cast<EValueTypes>(typeCode);
		prop.propName = propDescriptors.chunks[1];

		if (propDescriptors.count == 3) { // simple data
			prop.propValues.push_back(propDescriptors.chunks[2]);
		}
		else { // array
			prop.isArray = true;
			for (int i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
				prop.propValues.push_back(propDescriptors.chunks[i + 3]);
			}
		}

		instanceProps.push_back(prop);
	}

	return instanceProps;
};

template<ESDTypes E>
std::string FEncodeInstanceData(FTypeData<E>& inst) {
	std::vector<FSDInstanceProp> instProps;
	FSDManager::FSDInstanceAction<E>(inst, instProps, EInstanceAction::toString);

	std::string encodedInstance;

	std::char propDelimiter = ';';
	std::char tokenDelimiter = ',';
	string typeCode = std::to_string(static_cast<int>(E));
	string propName;

	encodedInstance.append(typeCode);
	encodedInstance.push_back(propDelimiter);

	for (auto prop : instProps) {
		encodedInstance.append(std::to_string(static_cast<int>(prop.propValueType)));
		encodedInstance.push_back(tokenDelimiter);
		encodedInstance.append(prop.propName);
		encodedInstance.push_back(tokenDelimiter);

		if (prop.isArray) {
			encodedInstance.append("isArray");
			encodedInstance.push_back(tokenDelimiter);

			for (auto value : prop.propValues) {
				encodedInstance.append(value);
				encodedInstance.push_back(tokenDelimiter);
			}
		}
		else {
			encodedInstance.append(prop.propValues[0]);
		}

		encodedInstance.push_back(propDelimiter);
	}

	encodedInstance.push_back('\n');
	return encodedInstance;
};

template<ESDTypes E>
void FSDSpecializedCall(ESDSpecializations spec) {
	if (spec == ESDSpecializations::createStaticData) {
		FTypeData<E> instanceStruct;
		std::vector<FSDInstanceProp> instanceProps = FDecodeInstanceData();
		FSDManager::FSDInstanceAction<E>(instanceStruct, instanceProps, EInstanceAction::toInstance);
		UE_LOG(LogTemp, Warning, TEXT("---------> Adding instance data to map..."));
	}
	else if (spec == ESDSpecializations::saveStaticData) {
		TMap<int32, FTypeData<E>> typeData = FSDManager::getTypeData<E>();

		std::string line;
		for (auto& inst : typeData)
		{
			line.append(FEncodeInstanceData<E>(inst.Value));
			line.push_back('\n');
			currentSaveFile << line;
			line.clear();
		}
	}
}

void FSDSpecializationJuncture(int typeCode, ESDSpecializations spec) {
	if (typeCode == static_cast<int>(ESDTypes::type1)) {
		FSDSpecializedCall<ESDTypes::type1>(spec);
	}
	else if (typeCode == static_cast<int>(ESDTypes::type2)) {
		FSDSpecializedCall<ESDTypes::type2>(spec);
	}
}

void SaveStaticData() {
	currentSaveFile.open("StaticDataOut.txt");
	//std::string line;

	for (int i = 0; i <= static_cast<int>(ESDTypes::type2); i++)
	{
		FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
	}

	currentSaveFile.close();
}

void FParseRawStaticData() {
	std::ifstream infile("StaticData.txt");

	while (std::getline(infile, currentReadLine))
	{
		int typeCode = static_cast<int>(currentReadLine[0]);
		FSDSpecializationJuncture(typeCode, ESDSpecializations::createStaticData);
	}
}

class FSDManager {
public:
	static FStaticData StaticData;

	static bool staticDataLoaded() {
		return StaticData.dataIsSet;
	};

	static void FSetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Reading StaticData."));
		FParseRawStaticData();
		StaticData.dataIsSet = true;
	}

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

	typedef std::vector<FSDInstanceProp>& P;
	typedef EInstanceAction A;
	template<ESDTypes E>
	static void FSDInstanceAction(FTypeData<E>& inst, P instanceProps, A action) {};
	template<> static void FSDInstanceAction<ESDTypes::type1>(FTypeData<ESDTypes::type1>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::toInstance) StaticData.type1.Add(inst.id, inst);
	};
	template<> static void FSDInstanceAction<ESDTypes::type2>(FTypeData<ESDTypes::type2>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::toInstance) StaticData.type2.Add(inst.id, inst);
	};
};
