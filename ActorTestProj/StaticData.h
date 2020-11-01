
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

	static FString CurrentInstanceString;

	static FString StaticDataFString;

	static bool StaticDataIsReady() {
		FString text = StaticData.dataIsReady ? "YES" : "NO";
		UE_LOG(LogTemp, Warning, TEXT("---------> Is data set?: %s"), *text);
		return StaticData.dataIsReady;
	};

	static void SetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Reading StaticData."));
		LoadStaticData();
	};

	static FString GetStaticDataSavePath() {
		return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("StaticData.txt"));
	};
	
	static FString GetStaticDataLoadPath() {
		return FPaths::Combine(FPaths::GameSourceDir(), FApp::GetProjectName(), TEXT("StaticData.txt"));
	};

	static void LoadStaticData() {
		FString filePath = GetStaticDataLoadPath();
		UE_LOG(LogTemp, Warning, TEXT("---------> Path to static data:\n%s"), *filePath);
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		FString FileContent;

		if (!FileManager.FileExists(*filePath)) {
			UE_LOG(LogTemp, Warning, TEXT("---------> StaticData.tx file does not exist."));
			return;
		}
		if (!FFileHelper::LoadFileToString(FileContent, *filePath, FFileHelper::EHashOptions::None)) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Could not load text from StaticData file."));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: Text From File: %s"), *FileContent);

		int32 delimPosIndex, symbolCount, typeCode;
		int32 minLineLength = 10; // Valid encoding of instance cannot be shorter
		FString delim = TEXT("\n");

		// Parsing text to extract chain of characters describing a single instance
		while (FileContent.Len() > 0) {
			delimPosIndex = FileContent.Find(delim);
			symbolCount = (delimPosIndex != -1) ? (delimPosIndex + 1) : FileContent.Len();
			UE_LOG(LogTemp, Warning, TEXT("---------> Delimiter position: %i"), delimPosIndex);
			if (symbolCount < minLineLength) FSDHelp::Throw(TEXT("Ill formatted line: "), CurrentInstanceString);

			CurrentInstanceString = FileContent.Left(symbolCount); // save fragment
			CurrentInstanceString.RemoveFromEnd(TEXT("\n")); // try to remove just in case

			FileContent.RightChopInline(symbolCount, true); // remove fragment from variable and free space
			FileContent.RemoveFromStart(TEXT("\n")); // try to remove just in case

			UE_LOG(LogTemp, Warning, TEXT("---------> Parsing line: %s"), *CurrentInstanceString);
			UE_LOG(LogTemp, Warning, TEXT("---------> Remaining file: %s"), *FileContent);

			typeCode = FCString::Atoi(*(CurrentInstanceString.Left(1))); // read type
			CurrentInstanceString.RightChopInline(3, true); // remove type -> typeCode + "," + ";" = 3 symbols

			FSDSpecializationJuncture(static_cast<int32>(typeCode), ESDSpecializations::createStaticData);
		}

		UE_LOG(LogTemp, Warning, TEXT("---------> Static data was loaded with success."));
		StaticData.dataIsReady = true;

		UE_LOG(LogTemp, Warning, TEXT("---------> Now saving data sjust for laughs!! LOL. Kernel gonna kill me."));
		SaveStaticData();
	};

	static void SaveStaticData(const bool overwrite = true, bool debug = false) {
		// First writing to string
		for (int i = 0; i <= static_cast<int>(ESDTypes::type2); i++) {
			FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
		}
		UE_LOG(LogTemp, Warning, TEXT("---------> File to save:\n%s"), *StaticDataFString);

		FString filePath = GetStaticDataLoadPath();
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		FString FileContent;

		if (!FileManager.FileExists(*filePath) || overwrite) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Static Data file already exists."));
			return;
		}
		if (FFileHelper::SaveStringToFile(StaticDataFString, *filePath)) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Successfully written to the text file"));
			if (debug) {
				UE_LOG(LogTemp, Warning, TEXT("---------> File contentents:\n%s"), *StaticDataFString);
			}
		} else {
			UE_LOG(LogTemp, Warning, TEXT("---------> Failed to write to StaticData file."));
		}

		StaticDataFString = TEXT("");
	};

	template<ESDTypes E>
	static void CreateStaticData() {
		FSDTypeData<E> instanceStruct;
		std::vector<FSDInstanceProp> instanceProps = GetPropFromString();
		FSDInstanceAction<E>(instanceStruct, instanceProps, ESDInstanceAction::loadingStaticData);
		UE_LOG(LogTemp, Warning, TEXT("---------> Adding instance data to map..."));
	};
	
	template<ESDTypes E>
	static void SaveStaticData() {
		TMap<int32, FSDTypeData<E>> typeData = GetTypeData<E>();

		FString line;
		for (auto& tuple : typeData)
		{
			FSDTypeData<E>& inst = tuple.Value;
			line = FSDHelp::StdStringToFstring(FEncodeInstanceData<E>(inst));
			StaticDataFString.Append(line);
		}
	};

	template<ESDTypes E>
	static void SpecializedCall(ESDSpecializations spec) {
		if (spec == ESDSpecializations::createStaticData) CreateStaticData<E>();
		else if (spec == ESDSpecializations::saveStaticData) SaveStaticData<E>();
	};

	template<ESDTypes E>
	static std::string FEncodeInstanceData(FSDTypeData<E>& inst) {
		std::vector<FSDInstanceProp> instProps;
		FSDInstanceAction<E>(inst, instProps, ESDInstanceAction::savingStaticData);

		std::string encodedInstance;

		char propDelimiter = ';';
		char tokenDelimiter = ',';
		std::string instanceTypeCode = std::to_string(static_cast<int>(E));
		std::string propName;

		encodedInstance.append(instanceTypeCode);
		encodedInstance.push_back(tokenDelimiter);
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
					UE_LOG(LogTemp, Warning, TEXT("---------> Iterating over one of the values: %s"), *FSDHelp::StdStringToFstring(value));
					encodedInstance.append(value);
					encodedInstance.push_back(tokenDelimiter);
				}
			}
			else {
				encodedInstance.append(prop.propValues[0]);
				encodedInstance.push_back(tokenDelimiter);
			}

			encodedInstance.push_back(propDelimiter);
		}

		encodedInstance.push_back('\n');
		return encodedInstance;
	};

	static std::vector<FSDInstanceProp> GetPropFromString() {
		FSDHelp::ExtractedChunks instancePropsChunks = FSDHelp::ExtractChunks(
			FSDHelp::FstringToStdString(CurrentInstanceString)
		);

		std::vector<FSDInstanceProp> instanceProps;

		// Parsing prop by prop
		for (std::string propChunk : instancePropsChunks.chunks) {
			FSDInstanceProp prop;
			UE_LOG(LogTemp, Warning, TEXT("---------> Parsing prop chunk: %s"), *FSDHelp::StdStringToFstring(propChunk));

			FSDHelp::ExtractedChunks propDescriptors = FSDHelp::ExtractChunks(propChunk, ',');

			int32 typeCode = FSDHelp::StdStringToInt32(propDescriptors.chunks[0]);
			prop.propValueType = static_cast<ESDValueTypes>(typeCode);
			prop.propName = propDescriptors.chunks[1];

			if (propDescriptors.count == 3) { // simple data
				prop.propValues.push_back(propDescriptors.chunks[2]);
			}
			else { // array
				prop.isArray = true;
				UE_LOG(LogTemp, Warning, TEXT("---------> About to write values for property containing array."));
				for (int i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
					UE_LOG(LogTemp, Warning, TEXT("---------> Writing array value into Static Data instance property: %s"), *FSDHelp::StdStringToFstring(propDescriptors.chunks[i + 3]));
					prop.propValues.push_back(propDescriptors.chunks[i + 3]);
				}
			}

			instanceProps.push_back(prop);
		}

		return instanceProps;
	};

	template<ESDTypes E>
	static FSDTypeData<E>& GetTypeInstanceData(int32 instanceId) {
		TMap<int32, FSDTypeData<E>> typeData = GetTypeData<E>();
		bool hasInstance = typeData.Contains(instanceId);
		if (!hasInstance) {
			UE_LOG(LogTemp, Warning, TEXT("---------> There was no item"));
		}
		return typeData[instanceId];
	};

	template<class T>
	static void PropertyAction(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, ESDInstanceAction action) {
		if (action == ESDInstanceAction::loadingStaticData) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Trying to write to instance prop: %s."), *FSDHelp::StdStringToFstring(propName));
			for (FSDInstanceProp prop : properties) {
				if (propName == prop.propName) {
					for (int32 i = 0; i < prop.propValues.size(); i++) {
						UE_LOG(LogTemp, Warning, TEXT("---------> Write to instance value: %s."), *FSDHelp::StdStringToFstring(prop.propValues[i]));
						FSDHelp::SetEntityValueFromStdString(instanceProperty, prop.propValues[i]);
					}
					return;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Prop of name: '%s' not found."), *FSDHelp::StdStringToFstring(propName));
		}
		else if (action == ESDInstanceAction::savingStaticData) {
			FSDInstanceProp prop;
			prop.propName = propName;
			prop.propValueType = FSDHelp::GetValueTypeFromType<T>();
			FSDHelp::ToStdStringAndAssign(instanceProperty, prop);

			properties.push_back(prop);
		}
	};

	/**
	 * =========================================================
	 * Those require modification with ESDTypes change.
	 */

	static void FSDSpecializationJuncture(int typeCode, ESDSpecializations spec) {
		auto test = [=](ESDTypes e) { return typeCode == static_cast<int>(e); };
		if			(test(ESDTypes::type1))		SpecializedCall<ESDTypes::type1>(spec);
		else if		(test(ESDTypes::type2))		SpecializedCall<ESDTypes::type2>(spec);
	};

	template<ESDTypes E>
	static TMap<int32, FSDTypeData<E>> GetTypeData() { return StaticData.type1; };
	template<> static TMap<int32, FSDTypeData<ESDTypes::type1>> GetTypeData<ESDTypes::type1>() { return StaticData.type1; };
	template<> static TMap<int32, FSDTypeData<ESDTypes::type2>> GetTypeData<ESDTypes::type2>() { return StaticData.type2; };

	typedef std::vector<FSDInstanceProp>& P;
	typedef ESDInstanceAction A;
	template<ESDTypes E>
	static void FSDInstanceAction(FSDTypeData<E>& inst, P instProps, A action) {};
	template<> static void FSDInstanceAction<ESDTypes::type1>(FSDTypeData<ESDTypes::type1>& inst, P instProps, A action) {
		PropertyAction(inst.id, "id", instProps, action);
		PropertyAction(inst.prop1, "prop1", instProps, action);
		PropertyAction(inst.prop2, "prop2", instProps, action);
		if (action == A::loadingStaticData) StaticData.type1.Add(inst.id, inst);
	};
	template<> static void FSDInstanceAction<ESDTypes::type2>(FSDTypeData<ESDTypes::type2>& inst, P instProps, A action) {
		PropertyAction(inst.id, "id", instProps, action);
		PropertyAction(inst.prop1, "prop1", instProps, action);
		PropertyAction(inst.prop2, "prop2", instProps, action);
		PropertyAction(inst.prop3, "prop3", instProps, action);
		PropertyAction(inst.prop4, "prop4", instProps, action);
		PropertyAction(inst.prop5, "prop5", instProps, action);
		PropertyAction(inst.prop6, "prop6", instProps, action);
		PropertyAction(inst.prop7, "prop7", instProps, action);
		PropertyAction(inst.prop8, "prop8", instProps, action);
		if (action == A::loadingStaticData) StaticData.type2.Add(inst.id, inst);
	};
};