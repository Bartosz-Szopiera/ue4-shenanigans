
#pragma once

#include <vector>
#include <string>
#include "StaticDataTypes.h"
#include "StaticDataUtil.h"

class FSDStore {
public:
	static FStaticData StaticData;

	static FString CurrentInstanceString;

	static FString StaticDataFString;

	/*static enum class ESDInstanceAction {
		savingStaticData,
		loadingStaticData,
	};

	static enum class ESDSpecializations {
		createStaticData,
		saveStaticData,
	};*/

	static bool StaticDataIsReady() {
		FString text = StaticData.dataIsReady ? "YES" : "NO";
		UE_LOG(LogTemp, Warning, TEXT("---------> Is data set?: %s"), *text);
		return StaticData.dataIsReady;
	};

	/*static void SetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> Reading StaticData."));
		LoadStaticData();
	};*/

	//static FString GetStaticDataSavePath() {
	//	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("StaticData.txt"));
	//};
	//
	//static FString GetStaticDataLoadPath() {
	//	return FPaths::Combine(FPaths::GameSourceDir(), FApp::GetProjectName(), TEXT("StaticData.txt"));
	//};

	//static void LoadStaticData() {
	//	FString filePath = GetStaticDataLoadPath();
	//	UE_LOG(LogTemp, Warning, TEXT("---------> Path to static data:\n%s"), *filePath);
	//	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	//	FString FileContent;

	//	if (!FileManager.FileExists(*filePath)) {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> StaticData.tx file does not exist."));
	//		return;
	//	}
	//	if (!FFileHelper::LoadFileToString(FileContent, *filePath, FFileHelper::EHashOptions::None)) {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Could not load text from StaticData file."));
	//		return;
	//	}

	//	UE_LOG(LogTemp, Warning, TEXT("---------> FileManipulation: Text From File: %s"), *FileContent);

	//	int32 delimPosIndex, symbolCount, typeCode;
	//	int32 minLineLength = 10; // Valid encoding of instance cannot be shorter
	//	FString delim = TEXT("\n");

	//	// Parsing text to extract chain of characters describing a single instance
	//	while (FileContent.Len() > 0) {
	//		delimPosIndex = FileContent.Find(delim);
	//		symbolCount = (delimPosIndex != -1) ? (delimPosIndex + 1) : FileContent.Len();
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Delimiter position: %i"), delimPosIndex);
	//		if (symbolCount < minLineLength) FSDUtil::Throw(TEXT("Ill formatted line: "), CurrentInstanceString);

	//		CurrentInstanceString = FileContent.Left(symbolCount); // save fragment
	//		CurrentInstanceString.RemoveFromEnd(TEXT("\n")); // try to remove just in case

	//		/**
	//		 * This could actually be slowing process down. Potentially remove later.
	//		 */
	//		FileContent.RightChopInline(symbolCount, true); // remove fragment from variable and free space
	//		FileContent.RemoveFromStart(TEXT("\n")); // try to remove just in case

	//		UE_LOG(LogTemp, Warning, TEXT("---------> Parsing line: %s"), *CurrentInstanceString);
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Remaining file: %s"), *FileContent);

	//		typeCode = FCString::Atoi(*(CurrentInstanceString.Left(1))); // read type
	//		CurrentInstanceString.RightChopInline(3, true); // remove type -> typeCode + "," + ";" = 3 symbols

	//		FSDSpecializationJuncture(static_cast<int32>(typeCode), ESDSpecializations::createStaticData);
	//	}

	//	UE_LOG(LogTemp, Warning, TEXT("---------> Static data was loaded with success."));
	//	StaticData.dataIsReady = true;

	//	UE_LOG(LogTemp, Warning, TEXT("---------> Now saving data sjust for laughs!! LOL. Kernel gonna kill me."));
	//	SaveStaticData();
	//};

	//static void SaveStaticData(const bool overwrite = true, bool debug = false) {
	//	// First writing to string
	//	for (int i = 0; i <= static_cast<int>(FSD::Types::type2); i++) {
	//		FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("---------> File to save:\n%s"), *StaticDataFString);

	//	FString filePath = GetStaticDataLoadPath();
	//	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	//	FString FileContent;

	//	if (!FileManager.FileExists(*filePath) || overwrite) {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Static Data file already exists."));
	//		return;
	//	}
	//	if (FFileHelper::SaveStringToFile(StaticDataFString, *filePath)) {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Successfully written to the text file"));
	//		if (debug) {
	//			UE_LOG(LogTemp, Warning, TEXT("---------> File contentents:\n%s"), *StaticDataFString);
	//		}
	//	} else {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Failed to write to StaticData file."));
	//	}

	//	StaticDataFString = TEXT("");
	//};

	/*template<FSD::Types E>
	static void CreateStaticData() {
		FSDTypeData<E> instanceStruct;
		std::vector<FSDInstanceProp> instanceProps = GetPropFromString();
		TypeAction(instanceStruct, instanceProps, ESDInstanceAction::loadingStaticData);
		UE_LOG(LogTemp, Warning, TEXT("---------> Adding instance data to map..."));
	};

	template<FSD::Types E>
	static void SaveStaticData() {
		TMap<int32, FSDTypeData<E>> typeData = GetTypeData<E>();

		FString line;
		for (auto& tuple : typeData)
		{
			FSDTypeData<E>& inst = tuple.Value;
			line = FSDUtil::StdStringToFstring(FEncodeInstanceData<E>(inst));
			StaticDataFString.Append(line);
		}
	};

	template<FSD::Types E>
	static void SpecializedCall(ESDSpecializations spec) {
		if (spec == ESDSpecializations::createStaticData) CreateStaticData<E>();
		else if (spec == ESDSpecializations::saveStaticData) SaveStaticData<E>();
	};*/

	/*template<FSD::Types E>
	static std::string FEncodeInstanceData(FSDTypeData<E>& inst) {
		std::vector<FSDInstanceProp> instProps;
		TypeAction(inst, instProps, ESDInstanceAction::savingStaticData);

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
					UE_LOG(LogTemp, Warning, TEXT("---------> Iterating over one of the values: %s"), *FSDUtil::StdStringToFstring(value));
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
	};*/

	//static std::vector<FSDInstanceProp> GetPropFromString() {
	//	FSDUtil::ExtractedChunks instancePropsChunks = FSDUtil::ExtractChunks(
	//		FSDUtil::FstringToStdString(CurrentInstanceString)
	//	);

	//	std::vector<FSDInstanceProp> instanceProps;

	//	// Parsing prop by prop
	//	for (std::string propChunk : instancePropsChunks.chunks) {
	//		FSDInstanceProp prop;
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Parsing prop chunk: %s"), *FSDUtil::StdStringToFstring(propChunk));

	//		FSDUtil::ExtractedChunks propDescriptors = FSDUtil::ExtractChunks(propChunk, ',');

	//		int32 typeCode = FSDUtil::StdStringToInt32(propDescriptors.chunks[0]);
	//		prop.propValueType = static_cast<FSD::ValueTypes>(typeCode);
	//		prop.propName = propDescriptors.chunks[1];

	//		if (propDescriptors.count == 3) { // simple data
	//			prop.propValues.push_back(propDescriptors.chunks[2]);
	//		}
	//		else { // array
	//			prop.isArray = true;
	//			UE_LOG(LogTemp, Warning, TEXT("---------> About to write values for property containing array."));
	//			for (int i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
	//				UE_LOG(LogTemp, Warning, TEXT("---------> Writing array value into Static Data instance property: %s"), *FSDUtil::StdStringToFstring(propDescriptors.chunks[i + 3]));
	//				prop.propValues.push_back(propDescriptors.chunks[i + 3]);
	//			}
	//		}

	//		instanceProps.push_back(prop);
	//	}

	//	return instanceProps;
	//};

	//template<class T>
	//// PropertyAction
	//static void PA(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, ESDInstanceAction action) {
	//	if (action == ESDInstanceAction::loadingStaticData) {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> Trying to write to instance prop: %s."), *FSDUtil::StdStringToFstring(propName));
	//		for (FSDInstanceProp prop : properties) {
	//			if (propName == prop.propName) {
	//				for (int32 i = 0; i < prop.propValues.size(); i++) {
	//					UE_LOG(LogTemp, Warning, TEXT("---------> Write to instance value: %s."), *FSDUtil::StdStringToFstring(prop.propValues[i]));
	//					FSDUtil::SetEntityValueFromStdString(instanceProperty, prop.propValues[i]);
	//				}
	//				return;
	//			}
	//		}
	//		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Prop of name: '%s' not found."), *FSDUtil::StdStringToFstring(propName));
	//	}
	//	else if (action == ESDInstanceAction::savingStaticData) {
	//		FSDInstanceProp prop;
	//		prop.propName = propName;
	//		prop.propValueType = FSDUtil::GetValueTypeFromType<T>();
	//		FSDUtil::ToStdStringAndAssign(instanceProperty, prop);

	//		properties.push_back(prop);
	//	}
	//};

	//template<FSD::Types E>
	//static FSDTypeData<E>& GetTypeInstanceData(int32 instanceId) {
	//	TMap<int32, FSDTypeData<E>> typeData = GetTypeData<E>();
	//	bool hasInstance = typeData.Contains(instanceId);
	//	if (!hasInstance) {
	//		UE_LOG(LogTemp, Warning, TEXT("---------> There was no item"));
	//	}
	//	return typeData[instanceId];
	//};

	/**
	 * ==========================================================================================
	 * UGLY GENERATED CODE
	 * ==========================================================================================
	 * Those require modification with Types change.
	 */

	/*static void FSDSpecializationJuncture(int typeCode, ESDSpecializations spec) {
		auto test = [=](FSD::Types e) { return typeCode == static_cast<int>(e); };
		if			(test(FSD::Types::type1))		SpecializedCall<FSD::Types::type1>(spec);
		else if		(test(FSD::Types::type2))		SpecializedCall<FSD::Types::type2>(spec);
	};*/

	/*template<FSD::Types E>
	static auto GetTypeData() { return StaticData.type1; };
	template<> static auto GetTypeData<FSD::Types::type1>() { return StaticData.type1; };
	template<> static auto GetTypeData<FSD::Types::type2>() { return StaticData.type2; };*/

	//typedef std::vector<FSDInstanceProp>& P; // Vector of instance properties
	//typedef ESDInstanceAction A; // Action to perform
	//static void TypeAction(FSDTypeData<FSD::Types::type1>& i /* i - instance */, P p, A a) {
	//	PA(i.id, "id", p, a);PA(i.prop1, "prop1", p, a);PA(i.prop2, "prop2", p, a);
	//	if (a == A::loadingStaticData) StaticData.type1.Add(i.id, i);
	//};
	//static void TypeAction(FSDTypeData<FSD::Types::type2>& i, P p, A a) {
	//	PA(i.id, "id", p, a);PA(i.prop1, "prop1", p, a);PA(i.prop2, "prop2", p, a);PA(i.prop3, "prop3", p, a);PA(i.prop4, "prop4", p, a);PA(i.prop5, "prop5", p, a);PA(i.prop6, "prop6", p, a);PA(i.prop7, "prop7", p, a);PA(i.prop8, "prop8", p, a);
	//	if (a == A::loadingStaticData) StaticData.type2.Add(i.id, i);
	//};
};