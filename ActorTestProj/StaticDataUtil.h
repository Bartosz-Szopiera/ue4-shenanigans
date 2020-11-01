
#pragma once

#include <string>
#include <vector>
#include "Misc/DefaultValueHelper.h"

class FSDUtil {
public:
	/**
	 * Casting from strings to value types
	 */
	static FString StdStringToFstring(std::string value) {
		return FString(value.c_str());
	};

	static int32 StdStringToInt32(std::string value) {
		FString fstr = StdStringToFstring(value);
		int32 out;
		FDefaultValueHelper::ParseInt(fstr, out);
		return out;
	};

	static float StdStringToFloat(std::string value) {
		std::string::size_type sz;   // alias of size_t
		return std::stof(value, &sz);
	};

	static bool StdStringToBool(std::string value) {
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
		return false;
	};

	/**
	 * Casting from value types to std::string
	 */
	static std::string Int32ToStdString(int32 source) {
		FString fstr;
		fstr = FString::FromInt(source);
		return std::string(TCHAR_TO_UTF8(*fstr));
	};

	static std::string FstringToStdString(FString source) {
		return std::string(TCHAR_TO_UTF8(*source));
	};

	static std::string FloatToStdString(float source) {
		FString fstr;
		fstr = FString::SanitizeFloat(source);
		return std::string(TCHAR_TO_UTF8(*fstr));
	};

	static std::string BoolToStdString(bool source) {
		return source ? "0" : "1";
	};

	/**
	 * 
	 */

	static void Throw(FString s1 = TEXT(""), FString s2 = TEXT(""), FString s3 = TEXT("")) {
		FString end = TEXT("\n---------------------------------------------\n");
		FString text = s1.Append(s2).Append(s3);
		UE_LOG(LogTemp, Warning, TEXT("%s---------> Exception:\n%s\n"), *end, *text);
		throw;
	};

	/**
	 * 
	 */

	static struct ExtractedChunks {
		std::vector<std::string> chunks;
		int32 count;
	};

	static ExtractedChunks ExtractChunks(std::string data, char delimiter = ';') {
		int32 chunkStart = 0;
		int32 chunkEnd = 0;
		ExtractedChunks extracted;
		extracted.count = 0;

		auto saveChunk = [&]() {
			int32 length = chunkEnd - chunkStart;
			UE_LOG(LogTemp, Warning, TEXT("---------> Extracting chunk: %s"), *FSDUtil::StdStringToFstring(data.substr(chunkStart, length)));
			extracted.chunks.push_back(data.substr(chunkStart, length));
			extracted.count++;
		};

		for (char c : data) {
			if (c == delimiter) {
				saveChunk();
				chunkEnd++;
				chunkStart = chunkEnd;
			}
			else {
				chunkEnd++;
			}
		};

		return extracted;
	};

	/**
	 * Used for loading SD
	 */
	template<class T>
	static T GetValueFromStdString(std::string source) { return StdStringToInt32(source); };
	template<> static int32 GetValueFromStdString<int32>(std::string source) { return StdStringToInt32(source); };
	template<> static float GetValueFromStdString<float>(std::string source) { return StdStringToFloat(source); };
	template<> static FString GetValueFromStdString<FString>(std::string source) { return StdStringToFstring(source); };
	template<> static bool GetValueFromStdString<bool>(std::string source) { return StdStringToBool(source); };

	/**
	 * Used for saving SD
	 */
	static std::string GetStringFromValue(int32 source) { return Int32ToStdString(source); };
	static std::string GetStringFromValue(float source) { return FloatToStdString(source); };
	static std::string GetStringFromValue(FString source) { return FstringToStdString(*source); };
	static std::string GetStringFromValue(bool source) { return BoolToStdString(source); };

	/**
	 * It's about getting prop value for the purpose of encoding in string
	 */
	template<class T>
	static void ToStdStringAndAssign(TArray<T> sourceProp, FSDInstanceProp& targetProp) {
		targetProp.isArray = true;
		UE_LOG(LogTemp, Warning, TEXT("---------> Recognized array"));
		std::string newVal;
		for (auto& value : sourceProp) {
			newVal = FSDUtil::GetStringFromValue(value);
			UE_LOG(LogTemp, Warning, TEXT("---------> Got value for array property: %s"), *FSDUtil::StdStringToFstring(newVal));
			targetProp.propValues.push_back(newVal);
		};
	};
	template<class T>
	static void ToStdStringAndAssign(T sourceProp, FSDInstanceProp& targetProp) {
		targetProp.isArray = false;
		targetProp.propValues.push_back(FSDUtil::GetStringFromValue(sourceProp));
	};

	/**
	 * Used for saving SD
	 * 
	 * Conditions need to be extended with each new Value Type
	 */
	template<class T>
	static ESDValueTypes GetValueTypeFromType() {
		if (std::is_same<T, int32>::value || std::is_same<T, TArray<int32>>::value)		return ESDValueTypes::int32;
		if (std::is_same<T, float>::value || std::is_same<T, TArray<float>>::value)		return ESDValueTypes::flt;
		if (std::is_same<T, FString>::value || std::is_same<T, TArray<FString>>::value)	return ESDValueTypes::string;
		if (std::is_same<T, bool>::value || std::is_same<T, TArray<bool>>::value)		return ESDValueTypes::boolean;
		Throw(TEXT("Could not match value type with instance property value. Defaults to int32."));
		return ESDValueTypes::int32;
	};

	/**
	 * USed for loading SD
	 * 
	 * This overloads need to be extended with each new Value Type
	 */
	template<class T>
	static void SetEntityValueFromStdString(TArray<T>& instanceProperty, std::string value) {
		T newItem;
		SetEntityValueFromStdString(newItem, value);
		instanceProperty.Add(newItem);
	};
	static void SetEntityValueFromStdString(int32& instanceProperty, std::string value) {
		instanceProperty = GetValueFromStdString<int32>(value);
	};
	static void SetEntityValueFromStdString(float& instanceProperty, std::string value) {
		instanceProperty = GetValueFromStdString<float>(value);
	};
	static void SetEntityValueFromStdString(FString& instanceProperty, std::string value) {
		instanceProperty = GetValueFromStdString<FString>(value);
	};
	static void SetEntityValueFromStdString(bool& instanceProperty, std::string value) {
		instanceProperty = GetValueFromStdString<bool>(value);
	};
};
