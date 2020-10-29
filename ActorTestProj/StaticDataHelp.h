#pragma once

#include <string>
#include <vector>
#include "Misc/DefaultValueHelper.h"

FString castStdStringToFstring(std::string value) {
	try
	{
		return FString(value.c_str());
	}
	catch (const std::exception& exc)
	{
		FString fstr1 = FString(value.c_str());
		//std::string excStr = *exc.what()
		//FString fstrExc = FString(excStr.c_str());
		//UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting '%s' to FString. Exception: %s"), value, exc.what());
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting '%s' to FString. Exception: %s"), *fstr1, *exc.what());
		return (FString)"0conversionError";
	}
};

int32 castStdStringToInt32(std::string value) {
	FString fstr = castStdStringToFstring(value);
	int32 out;
	FDefaultValueHelper::ParseInt(fstr, out);
	return out;
};

float castStdStringToFloat(std::string value) {
	std::string::size_type sz;   // alias of size_t
	return std::stof(value, &sz);
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
};

struct extractedChunks {
	std::vector<std::string> chunks;
	int32 count;
};

extractedChunks extractChunks(std::string data, char delimiter = ';') {
	int32 chunkStart = 0;
	int32 chunkEnd = 0;
	extractedChunks extracted;
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
