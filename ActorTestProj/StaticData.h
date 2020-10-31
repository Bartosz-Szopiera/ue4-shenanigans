
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "StaticDataTypes.h"
#include "StaticDataHelp.h"

class FSDManager {
public:
	static FStaticData FSDStaticData;

	static FString FSDCurrentReadLine;

	//static std::ofstream FSDCurrentSaveFile;
	static FString FSDCurrentSaveFile;

	static bool staticDataLoaded() {
		FString text = FSDStaticData.dataIsSet ? "YES" : "NO";
		UE_LOG(LogTemp, Warning, TEXT("---------> Is data set?: %s"), *text);
		return FSDStaticData.dataIsSet;
	};

	static void FSetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Reading StaticData."));
		FParseRawStaticData();
	};

	static void FParseRawStaticData() {
		FString filePath = FPaths::Combine(FPaths::GameSourceDir(), FApp::GetProjectName(), TEXT("StaticData.txt"));
		UE_LOG(LogTemp, Warning, TEXT("---------> Combined file path:\n%s"), *filePath);
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		FString FileContent;

		if (FileManager.FileExists(*filePath))
		{
			if (FFileHelper::LoadFileToString(FileContent, *filePath, FFileHelper::EHashOptions::None))
			{
				UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: Text From File: %s"), *FileContent);

				int32 delimPosition;
				int32 fragmentLength;
				int32 typeCode;

				while (FileContent.Len() > 0) {
					// No instance can be encoded in less than 9 symbols

					// If file ends without end-line get length
					delimPosition = FileContent.Find(TEXT("\n"));
					fragmentLength = (delimPosition != -1) ? (delimPosition + 1) : FileContent.Len();

					UE_LOG(LogTemp, Warning, TEXT("---------> Delimiter position: %i"), delimPosition);

					// Get characters from left and remove end-line if it was there
					FSDCurrentReadLine = FileContent.Left(fragmentLength);
					FSDCurrentReadLine.RemoveFromEnd(TEXT("\n"));

					if (FSDCurrentReadLine.Len() < 9) { FSDHelp::FThrow(TEXT("Ill formatted line: "), FSDCurrentReadLine); };

					FileContent.RightChopInline(FSDCurrentReadLine.Len(), true);

					// Now delimited is on the beginning and me need to remove it!
					FileContent.RemoveFromStart(TEXT("\n"));

					UE_LOG(LogTemp, Warning, TEXT("---------> Parsing line: %s"), *FSDCurrentReadLine);
					UE_LOG(LogTemp, Warning, TEXT("---------> Remaining file: %s"), *FileContent);

					// Read type and remove it and next 2 delimiters from line
					typeCode = FCString::Atoi(*(FSDCurrentReadLine.Left(1)));
					FSDCurrentReadLine.RightChopInline(3, true);

					FSDSpecializationJuncture(static_cast<int32>(typeCode), ESDSpecializations::createStaticData);
				}

				// Announce that static data is ready
				UE_LOG(LogTemp, Warning, TEXT("---------> Announcing that static data is ready"));
				FSDStaticData.dataIsSet = true;

				UE_LOG(LogTemp, Warning, TEXT("---------> Now saving data sjust for laughs!! LOL. Kernel gonna kill me."));
				SaveStaticData();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: Did not load text from file"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: File does not exist"));
		}
	};

	static void SaveStaticData() {
		// First writing to string
		for (int i = 0; i <= static_cast<int>(ESDTypes::type2); i++)
		{
			FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
		}

		UE_LOG(LogTemp, Warning, TEXT("---------> File to save:\n%s"), *FSDCurrentSaveFile);

		FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("StaticData.txt"));
		UE_LOG(LogTemp, Warning, TEXT("---------> Combined file path:\n%s"), *filePath);
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		FString FileContent;

		if (FileManager.FileExists(*filePath))
		{
			// We use the LoadFileToString to load the file into
			if (FFileHelper::SaveStringToFile(FSDCurrentSaveFile, *filePath))
			{
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Sucsesfuly Written: \"%s\" to the text file"), *FSDCurrentSaveFile);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *filePath);
		}

		FSDCurrentSaveFile = TEXT("");
	};

	template<ESDTypes E>
	static void FSDSpecializedCall(ESDSpecializations spec) {
		if (spec == ESDSpecializations::createStaticData) {
			FSDTypeData<E> instanceStruct;
			std::vector<FSDInstanceProp> instanceProps = FDecodeInstanceData();
			FSDInstanceAction<E>(instanceStruct, instanceProps, ESDInstanceAction::writeToInstance);
			UE_LOG(LogTemp, Warning, TEXT("---------> Adding instance data to map..."));
		}
		else if (spec == ESDSpecializations::saveStaticData) {
			TMap<int32, FSDTypeData<E>> typeData = GetTypeData<E>();

			FString line;
			for (auto& tuple : typeData)
			{
				FSDTypeData<E>& inst = tuple.Value;
				line = FSDHelp::FSDCastStdStringToFstring(FEncodeInstanceData<E>(inst));
				FSDCurrentSaveFile.Append(line);
			}
		}
	};

	template<ESDTypes E>
	static std::string FEncodeInstanceData(FSDTypeData<E>& inst) {
		std::vector<FSDInstanceProp> instProps;
		FSDInstanceAction<E>(inst, instProps, ESDInstanceAction::writeToString);

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
					UE_LOG(LogTemp, Warning, TEXT("---------> Iterating over one of the values: %s"), *FSDHelp::FSDCastStdStringToFstring(value));
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

	static std::vector<FSDInstanceProp> FDecodeInstanceData() {
		//FString encodedInstance = FSDCurrentReadLine;
		FSDHelp::FSDExtractedChunks instancePropsChunks = FSDHelp::FSDExtractChunks(
			FSDHelp::FSDCastFstringToStdString(FSDCurrentReadLine)
		);

		std::vector<FSDInstanceProp> instanceProps;

		// Parsing prop by prop
		for (std::string propChunk : instancePropsChunks.chunks) {
			FSDInstanceProp prop;
			UE_LOG(LogTemp, Warning, TEXT("---------> Parsing prop chunk: %s"), *FSDHelp::FSDCastStdStringToFstring(propChunk));

			FSDHelp::FSDExtractedChunks propDescriptors = FSDHelp::FSDExtractChunks(propChunk, ',');

			int32 typeCode = FSDHelp::FSDCastStdStringToInt32(propDescriptors.chunks[0]);
			prop.propValueType = static_cast<ESDValueTypes>(typeCode);
			prop.propName = propDescriptors.chunks[1];

			if (propDescriptors.count == 3) { // simple data
				prop.propValues.push_back(propDescriptors.chunks[2]);
			}
			else { // array
				prop.isArray = true;
				UE_LOG(LogTemp, Warning, TEXT("---------> About to write values for property containing array."));
				for (int i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
					UE_LOG(LogTemp, Warning, TEXT("---------> Writing array value into Static Data instance property: %s"), *FSDHelp::FSDCastStdStringToFstring(propDescriptors.chunks[i + 3]));
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
	static void FSDPropertyAction(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, ESDInstanceAction action) {
		if (action == ESDInstanceAction::writeToInstance) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Trying to write to instance prop: %s."), *FSDHelp::FSDCastStdStringToFstring(propName));
			for (FSDInstanceProp prop : properties) {
				if (propName == prop.propName) {
					for (int32 i = 0; i < prop.propValues.size(); i++) {
						UE_LOG(LogTemp, Warning, TEXT("---------> Write to instance value: %s."), *FSDHelp::FSDCastStdStringToFstring(prop.propValues[i]));
						FSDHelp::FSDSetInstancePropertyFromString(instanceProperty, prop.propValues[i]);
					}
					return;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Prop of name not found."));
		}
		else if (action == ESDInstanceAction::writeToString) {
			FSDInstanceProp prop;
			prop.propName = propName;
			prop.propValueType = FSDHelp::FSDGetValueTypeFromInstProp<T>();
			FSDHelp::FSDSetPropValueFromInstanceProp(instanceProperty, prop);

			properties.push_back(prop);
		}
	};

	/**
	 * =========================================================
	 * Those require modification with ESDTypes change.
	 */

	static void FSDSpecializationJuncture(int typeCode, ESDSpecializations spec) {
		if (typeCode == static_cast<int>(ESDTypes::type1)) {
			FSDSpecializedCall<ESDTypes::type1>(spec);
		}
		else if (typeCode == static_cast<int>(ESDTypes::type2)) {
			FSDSpecializedCall<ESDTypes::type2>(spec);
		}
	};

	template<ESDTypes E>
	static TMap<int32, FSDTypeData<E>> GetTypeData() { return FSDStaticData.type1; };
	template<> static TMap<int32, FSDTypeData<ESDTypes::type1>> GetTypeData<ESDTypes::type1>() { return FSDStaticData.type1; };
	template<> static TMap<int32, FSDTypeData<ESDTypes::type2>> GetTypeData<ESDTypes::type2>() { return FSDStaticData.type2; };

	typedef std::vector<FSDInstanceProp>& P;
	typedef ESDInstanceAction A;
	template<ESDTypes E>
	static void FSDInstanceAction(FSDTypeData<E>& inst, P instProps, A action) {};
	template<> static void FSDInstanceAction<ESDTypes::type1>(FSDTypeData<ESDTypes::type1>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::writeToInstance) FSDStaticData.type1.Add(inst.id, inst);
	};
	template<> static void FSDInstanceAction<ESDTypes::type2>(FSDTypeData<ESDTypes::type2>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.prop3, "prop3", instProps, action);
		FSDPropertyAction(inst.prop4, "prop4", instProps, action);
		FSDPropertyAction(inst.prop5, "prop5", instProps, action);
		FSDPropertyAction(inst.prop6, "prop6", instProps, action);
		FSDPropertyAction(inst.prop7, "prop7", instProps, action);
		FSDPropertyAction(inst.prop8, "prop8", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::writeToInstance) FSDStaticData.type2.Add(inst.id, inst);
	};
};