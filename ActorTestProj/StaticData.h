
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

	static std::ofstream FSDCurrentSaveFile;

	static bool staticDataLoaded() {
		FString text = FSDStaticData.dataIsSet ? "YES" : "NO";
		UE_LOG(LogTemp, Warning, TEXT("---------> Is data set?: %s"), *text);
		return FSDStaticData.dataIsSet;
	};

	static void FSetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Reading StaticData."));
		try {
			FParseRawStaticData();
		}
		catch (std::exception& e) {
			UE_LOG(LogTemp, Fatal, TEXT("---------> Error when parsing static data:\n %s."), e.what());
		}
	};

	static void FParseRawStaticData() {
		//std::ifstream infile("StaticData.txt");

		//while (std::getline(infile, FSDCurrentReadLine))
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("---------> Reading line: %s."), *FSDHelp::FSDCastStdStringToFstring(FSDCurrentReadLine));
		//	if (FSDCurrentReadLine.size() < 9) FSDHelp::FThrow("Ill formatted line: \n", FSDCurrentReadLine);

		//	int typeCode = static_cast<int>(FSDCurrentReadLine[0]);
		//	FSDCurrentReadLine.erase(FSDCurrentReadLine[0]);
		//	FSDSpecializationJuncture(typeCode, ESDSpecializations::createStaticData);
		//}
		////FSDStaticData.dataIsSet = true;
		//UE_LOG(LogTemp, Warning, TEXT("---------> Successfully parsed static data."));

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

				while (FileContent.Len() > 0) {
					// No instance can be encoded in less than 9 symbols

					// If file ends without end-line get length
					delimPosition = FileContent.Find(TEXT("\n"));
					fragmentLength = (delimPosition != -1) ? (delimPosition + 1) : FileContent.Len();

					UE_LOG(LogTemp, Warning, TEXT("---------> Delimiter position: %i"), delimPosition);

					// Get characters from left and remove end-line if it was there
					FSDCurrentReadLine = FileContent.Left(fragmentLength);

					if (FSDCurrentReadLine.Len() < 9) { FSDHelp::FThrow(TEXT("Ill formatted line: "), FSDCurrentReadLine); };

					FileContent.RightChopInline(FSDCurrentReadLine.Len(), true);

					// Now delimited is on the beginning and me need to remove it!
					FileContent.RemoveFromStart(TEXT("\n"));

					UE_LOG(LogTemp, Warning, TEXT("---------> Parsing line: %s"), *FSDCurrentReadLine);
					UE_LOG(LogTemp, Warning, TEXT("---------> Remaining file: %s"), *FileContent);

					int32 typeCode = static_cast<int32>(FCString::Atoi(*(FSDCurrentReadLine.RightChop(1))));
					FSDSpecializationJuncture(typeCode, ESDSpecializations::createStaticData);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: Did not load text from file"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: File does not exist"));
		}


		//FString fn = "C:\\Users\\Bartosz\\Documents\\Unreal Projects\\ActorTestProj\\Source\\ActorTestProj\\StaticData";
		//const TCHAR* fileName = *fn;
		/**
			None                = 0,
			NoFail              = 1 << 0,
			Silent              = 1 << 1,
			AllowWrite          = 1 << 2
		 */
		//FFileHelper::LoadFileToString(FileContent, fileName, FFileHelper::EHashOptions::EnableVerify, 1);
		//UE_LOG(LogTemp, Warning, TEXT("---------> Successfully read static data file: \n%s"), *fileName);
	};

	static void SaveStaticData() {
		FSDCurrentSaveFile.open("StaticDataOut.txt");

		for (int i = 0; i <= static_cast<int>(ESDTypes::type2); i++)
		{
			FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
		}

		FSDCurrentSaveFile.close();
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

			std::string line;
			for (auto& inst : typeData)
			{
				line.append(FEncodeInstanceData<E>(inst.Value));
				line.push_back('\n');
				FSDCurrentSaveFile << line;
				line.clear();
			}
		}
	};

	template<ESDTypes E>
	static std::string FEncodeInstanceData(FSDTypeData<E>& inst) {
		std::vector<FSDInstanceProp> instProps;
		FSDManager::FSDInstanceAction<E>(inst, instProps, ESDInstanceAction::writeToString);

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
				for (int i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
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
			for (FSDInstanceProp prop : properties) {
				if (propName == prop.propName) {
					for (int32 i = 0; i < prop.propValues.size(); i++) {
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
	static void FSDInstanceAction(FSDTypeData<E>& inst, P instanceProps, A action) {};
	template<> static void FSDInstanceAction<ESDTypes::type1>(FSDTypeData<ESDTypes::type1>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::writeToInstance) FSDStaticData.type1.Add(inst.id, inst);
	};
	template<> static void FSDInstanceAction<ESDTypes::type2>(FSDTypeData<ESDTypes::type2>& inst, P instProps, A action) {
		FSDPropertyAction(inst.prop1, "prop1", instProps, action);
		FSDPropertyAction(inst.prop2, "prop2", instProps, action);
		FSDPropertyAction(inst.id, "id", instProps, action);
		if (action == A::writeToInstance) FSDStaticData.type2.Add(inst.id, inst);
	};
};