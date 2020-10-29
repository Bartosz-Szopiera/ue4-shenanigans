#pragma once

FString castStdStringToFstring(std::string value) {
	try
	{
		return FString(value.c_str());
	}
	catch (const std::exception& exc)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"%s\" to FString. Exception:\n%s"), value, exc.what());
		return (FString)"0conversionError";
	}
};

int32 castStdStringToInt32(std::string value) {
	try
	{
		FString fstr = castStdStringToFstring(value);
		int32 out;
		FDefaultValueHelper::ParseInt(fstr, out);
		return out;
	}
	catch (const std::exception& exc)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"%s\" to int32. Exception:\n%s"), value, exc.what());
		return 0;
	}
};

float castStdStringToFloat(std::string value) {
	try
	{
		std::string::size_type sz;   // alias of size_t
		return std::stof(value, &sz);
	}
	catch (const std::exception& exc)
	{
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"%s\" to float. Exception:\n%s"), value, exc.what());
		return 0;
	}
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
	else {
		UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Failed casting \"%s\" to bool."), value);
		return false;
	}
};

struct extractedChunks {
	std::vector<std::string> chunks;
	int32 count;
};
