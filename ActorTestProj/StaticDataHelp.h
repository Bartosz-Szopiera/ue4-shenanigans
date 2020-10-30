#pragma once

#include <string>
#include <vector>
#include "Misc/DefaultValueHelper.h"

FString castStdStringToFstring(std::string value) {
	return FString(value.c_str());
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
	return false;
};

std::string castInt32ToStdString(int32 source) {
	FString fstr;
	fstr = FString::FromInt(source);
	return std::string(TCHAR_TO_UTF8(*fstr));
};

std::string castFstringToStdString(FString source) {
	FString fstr;
	return std::string(TCHAR_TO_UTF8(*source));
};

std::string castFloatToStdString(float source) {
	FString fstr;
	fstr = FString::SanitizeFloat(source);
	return std::string(TCHAR_TO_UTF8(*fstr));
};

std::string castBoolToStdString(bool source) {
	return source ? "0" : "1";
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
