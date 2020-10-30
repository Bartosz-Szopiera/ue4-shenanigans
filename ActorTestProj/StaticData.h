
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "StaticDataTypes.h"
#include "StaticDataHelp.h"

class FSDManager {
public:
	static FStaticData StaticData;

	static std::string SDCurrentReadLine;

	static std::ofstream SDCurrentSaveFile;

	static bool staticDataLoaded() {
		return StaticData.dataIsSet;
	};

	static void FSetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Reading StaticData."));
		FParseRawStaticData();
		StaticData.dataIsSet = true;
	};

	static void FParseRawStaticData() {
		std::ifstream infile("StaticData.txt");

		while (std::getline(infile, SDCurrentReadLine))
		{
			int typeCode = static_cast<int>(SDCurrentReadLine[0]);
			FSDSpecializationJuncture(typeCode, ESDSpecializations::createStaticData);
		}
	};

	static void SaveStaticData() {
		SDCurrentSaveFile.open("StaticDataOut.txt");

		for (int i = 0; i <= static_cast<int>(ESDTypes::type2); i++)
		{
			FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
		}

		SDCurrentSaveFile.close();
	};

	static void FSDSpecializationJuncture(int typeCode, ESDSpecializations spec) {
		if (typeCode == static_cast<int>(ESDTypes::type1)) {
			FSDSpecializedCall<ESDTypes::type1>(spec);
		}
		else if (typeCode == static_cast<int>(ESDTypes::type2)) {
			FSDSpecializedCall<ESDTypes::type2>(spec);
		}
	};

	template<ESDTypes E>
	static void FSDSpecializedCall(ESDSpecializations spec) {
		if (spec == ESDSpecializations::createStaticData) {
			FTypeData<E> instanceStruct;
			std::vector<FSDInstanceProp> instanceProps = FDecodeInstanceData();
			FSDInstanceAction<E>(instanceStruct, instanceProps, EInstanceAction::writeToInstance);
			UE_LOG(LogTemp, Warning, TEXT("---------> Adding instance data to map..."));
		}
		else if (spec == ESDSpecializations::saveStaticData) {
			TMap<int32, FTypeData<E>> typeData = GetTypeData<E>();

			std::string line;
			for (auto& inst : typeData)
			{
				line.append(FEncodeInstanceData<E>(inst.Value));
				line.push_back('\n');
				SDCurrentSaveFile << line;
				line.clear();
			}
		}
	};

	template<ESDTypes E>
	static std::string FEncodeInstanceData(FTypeData<E>& inst) {
		std::vector<FSDInstanceProp> instProps;
		FSDManager::FSDInstanceAction<E>(inst, instProps, EInstanceAction::writeToString);

		std::string encodedInstance;

		char propDelimiter = ';';
		char tokenDelimiter = ',';
		std::string typeCode = std::to_string(static_cast<int>(E));
		std::string propName;

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

	static std::vector<FSDInstanceProp> FDecodeInstanceData() {
		std::string encodedInstance = SDCurrentReadLine;
		encodedInstance.erase(encodedInstance.begin());
		FSDExtractedChunks instancePropsChunks = FSDExtractChunks(encodedInstance);

		std::vector<FSDInstanceProp> instanceProps;

		// Parsing prop by prop
		for (std::string propChunk : instancePropsChunks.chunks) {
			FSDInstanceProp prop;

			FSDExtractedChunks propDescriptors = FSDExtractChunks(propChunk, ',');

			//prop.propValueType = static_cast<EValueTypes>(static_cast<int>(propDescriptors.chunks[0]));
			int32 typeCode = FSDCastStdStringToInt32(propDescriptors.chunks[0]);
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
	static FTypeData<E>& GetTypeInstanceData(int32 instanceId) {
		TMap<int32, FTypeData<E>> typeData = GetTypeData<E>();
		bool hasInstance = typeData.Contains(instanceId);
		if (!hasInstance) {
			UE_LOG(LogTemp, Warning, TEXT("---------> There was no item"));
		}
		return typeData[instanceId];
	};

	// It's about getting prop value for the purpose of encoding in string
	template<class T>
	static void FSDSetPropValueFromInstanceProp(TArray<T> source, FSDInstanceProp& prop) {
		prop.isArray = true;
		for (T& value : source) { prop.propValues.push_back(FSDGetStringFromValue(value)); };
	};
	template<class T>
	static void FSDSetPropValueFromInstanceProp(T source, FSDInstanceProp& prop) {
		prop.isArray = false;
		prop.propValues.push_back(FSDGetStringFromValue(source));
	};

	template<class T>
	static EValueTypes FSDGetValueTypeFromInstProp() { return EValueTypes::int32; };
	template<> static EValueTypes FSDGetValueTypeFromInstProp<int32>() { return EValueTypes::int32; };
	template<> static EValueTypes FSDGetValueTypeFromInstProp<float>() { return EValueTypes::flt; };
	template<> static EValueTypes FSDGetValueTypeFromInstProp<FString>() { return EValueTypes::string; };
	template<> static EValueTypes FSDGetValueTypeFromInstProp<bool>() { return EValueTypes::boolean; };

	template<class T>
	static void FSDPropertyAction(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, EInstanceAction action) {
		if (action == EInstanceAction::writeToInstance) {
			for (FSDInstanceProp prop : properties) {
				if (propName == prop.propName) {
					for (int32 i = 0; i < prop.propValues.size(); i++) {
						FSDSetInstancePropertyFromString(instanceProperty, prop.propValues[i]);
					}
					return;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Prop of name not found."));
		}
		else if (action == EInstanceAction::writeToString) {
			FSDInstanceProp prop;
			prop.propName = propName;
			prop.propValueType = FSDGetValueTypeFromInstProp<T>();
			FSDSetPropValueFromInstanceProp(instanceProperty, prop);

			properties.push_back(prop);
		}
	};

	/**
	 * =========================================================
	 */

	template<ESDTypes E>
	static TMap<int32, FTypeData<E>> GetTypeData() { return StaticData.type1; };
	template<> static TMap<int32, FTypeData<ESDTypes::type1>> GetTypeData<ESDTypes::type1>() { return StaticData.type1; };
	template<> static TMap<int32, FTypeData<ESDTypes::type2>> GetTypeData<ESDTypes::type2>() { return StaticData.type2; };

	typedef std::vector<FSDInstanceProp>& P;
	typedef EInstanceAction A;
	template<ESDTypes E>
	static void FSDInstanceAction(FTypeData<E>& inst, P instanceProps, A action) {};
	template<> static void FSDInstanceAction<ESDTypes::type1>(FTypeData<ESDTypes::type1>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::writeToInstance) StaticData.type1.Add(inst.id, inst);
	};
	template<> static void FSDInstanceAction<ESDTypes::type2>(FTypeData<ESDTypes::type2>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::writeToInstance) StaticData.type2.Add(inst.id, inst);
	};
};