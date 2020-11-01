
#pragma once

#include "SDCore.h"
#include "SDJuncture.gen.h"

class FSDFSActions {
public:
	/*static FString CurrentInstanceString;

	static FString StaticDataFString;*/

	static FString GetStaticDataSavePath() {
		return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("StaticData.txt"));
	};

	static FString GetStaticDataLoadPath() {
		return FPaths::Combine(FPaths::GameSourceDir(), FApp::GetProjectName(), TEXT("StaticData.txt"));
	};

	/*static void SetStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> About to load StaticData."));
		LoadStaticData();
	};*/

	static void LoadStaticData() {
		UE_LOG(LogTemp, Warning, TEXT("---------> About to load StaticData."));
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
			if (symbolCount < minLineLength) FSDUtil::Throw(TEXT("Ill formatted line: "), FSDCore::CurrentInstanceString);

			FSDCore::CurrentInstanceString = FileContent.Left(symbolCount); // save fragment
			FSDCore::CurrentInstanceString.RemoveFromEnd(TEXT("\n")); // try to remove just in case

			/**
			 * This could actually be slowing process down. Potentially remove later.
			 */
			FileContent.RightChopInline(symbolCount, true); // remove fragment from variable and free space
			FileContent.RemoveFromStart(TEXT("\n")); // try to remove just in case

			UE_LOG(LogTemp, Warning, TEXT("---------> Parsing line: %s"), *FSDCore::CurrentInstanceString);
			UE_LOG(LogTemp, Warning, TEXT("---------> Remaining file: %s"), *FileContent);

			typeCode = FCString::Atoi(*(FSDCore::CurrentInstanceString.Left(1))); // read type
			FSDCore::CurrentInstanceString.RightChopInline(3, true); // remove type -> typeCode + "," + ";" = 3 symbols

			FSDJuncture::FSDSpecializationJuncture(static_cast<int32>(typeCode), ESDSpecializations::createStaticData);
		}

		UE_LOG(LogTemp, Warning, TEXT("---------> Static data was loaded with success."));
		FSDCore::StaticData.dataIsReady = true;

		UE_LOG(LogTemp, Warning, TEXT("---------> Now saving data sjust for laughs!! LOL. Kernel gonna kill me."));
		SaveStaticData();
	};

	static void SaveStaticData(const bool overwrite = true, bool debug = false) {
		// First writing to string
		for (int i = 0; i <= static_cast<int>(FSD::Types::type2); i++) {
			FSDJuncture::FSDSpecializationJuncture(i, ESDSpecializations::saveStaticData);
		}
		UE_LOG(LogTemp, Warning, TEXT("---------> File to save:\n%s"), *FSDCore::StaticDataFString);

		FString filePath = GetStaticDataLoadPath();
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		FString FileContent;

		if (!FileManager.FileExists(*filePath) || overwrite) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Static Data file already exists."));
			return;
		}
		if (FFileHelper::SaveStringToFile(FSDCore::StaticDataFString, *filePath)) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Successfully written to the text file"));
			if (debug) {
				UE_LOG(LogTemp, Warning, TEXT("---------> File contentents:\n%s"), *FSDCore::StaticDataFString);
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("---------> Failed to write to StaticData file."));
		}

		FSDCore::StaticDataFString = TEXT("");
	};
};