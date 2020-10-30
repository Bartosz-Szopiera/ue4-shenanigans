
#pragma once

#include <string>
#include <vector>
#include "Misc/DefaultValueHelper.h"

class FSDHelp {
public:
	static FString FSDCastStdStringToFstring(std::string value) {
		return FString(value.c_str());
	};

	static int32 FSDCastStdStringToInt32(std::string value) {
		FString fstr = FSDCastStdStringToFstring(value);
		int32 out;
		FDefaultValueHelper::ParseInt(fstr, out);
		return out;
	};

	static float FSDCastStdStringToFloat(std::string value) {
		std::string::size_type sz;   // alias of size_t
		return std::stof(value, &sz);
	};

	static bool FSDCastStdStringToBool(std::string value) {
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

	static std::string FSDCastInt32ToStdString(int32 source) {
		FString fstr;
		fstr = FString::FromInt(source);
		return std::string(TCHAR_TO_UTF8(*fstr));
	};

	static std::string FSDCastFstringToStdString(FString source) {
		FString fstr;
		return std::string(TCHAR_TO_UTF8(*source));
	};

	static std::string FSDCastFloatToStdString(float source) {
		FString fstr;
		fstr = FString::SanitizeFloat(source);
		return std::string(TCHAR_TO_UTF8(*fstr));
	};

	static std::string FSDCastBoolToStdString(bool source) {
		return source ? "0" : "1";
	};

	static struct FSDExtractedChunks {
		std::vector<std::string> chunks;
		int32 count;
	};

	static FSDExtractedChunks FSDExtractChunks(std::string data, char delimiter = ';') {
		int32 chunkStart = 0;
		int32 chunkEnd = 0;
		FSDExtractedChunks extracted;
		extracted.count = 0;

		auto saveChunk = [&]() {
			int32 length = chunkEnd - chunkStart;
			extracted.chunks.push_back(data.substr(chunkStart, length));
			extracted.count++;
		};

		for (char c : data) {
			if (c == delimiter) {
				saveChunk();
				chunkStart = chunkEnd;
			}
			chunkEnd++;
		};

		return extracted;
	};

	template<class T>
	static T FSDGetValueFromString(std::string source) { return FSDCastStdStringToInt32(source); };
	template<> static int32 FSDGetValueFromString<int32>(std::string source) { return FSDCastStdStringToInt32(source); };
	template<> static float FSDGetValueFromString<float>(std::string source) { return FSDCastStdStringToFloat(source); };
	template<> static FString FSDGetValueFromString<FString>(std::string source) { return FSDCastStdStringToFstring(source); };
	template<> static bool FSDGetValueFromString<bool>(std::string source) { return FSDCastStdStringToBool(source); };

	static std::string FSDGetStringFromValue(int32 source) { return FSDCastInt32ToStdString(source); };
	static std::string FSDGetStringFromValue(float source) { return FSDCastFloatToStdString(source); };
	static std::string FSDGetStringFromValue(FString source) { return FSDCastFstringToStdString(*source); };
	static std::string FSDGetStringFromValue(bool source) { return FSDCastBoolToStdString(source); };

	template<class T>
	static void FSDSetInstancePropertyFromString(TArray<T>& instanceProperty, std::string value) {
		T newItem;
		FSDSetInstancePropertyFromString(newItem, value);
		instanceProperty.Append(newItem);
	};
	static void FSDSetInstancePropertyFromString(int32& instanceProperty, std::string value) {
		instanceProperty = FSDGetValueFromString<int32>(value);
	};
	static void FSDSetInstancePropertyFromString(float& instanceProperty, std::string value) {
		instanceProperty = FSDGetValueFromString<float>(value);
	};
	static void FSDSetInstancePropertyFromString(FString& instanceProperty, std::string value) {
		instanceProperty = FSDGetValueFromString<FString>(value);
	};
	static void FSDSetInstancePropertyFromString(bool& instanceProperty, std::string value) {
		instanceProperty = FSDGetValueFromString<bool>(value);
	};
};
