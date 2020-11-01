#pragma once

#include "SDCore.h"
#include "SDTypeAction.gen.h"
#include "SDGetTypeData.gen.h"

class STDCalls {
public:
	template<FSD::Types E>
	static void SpecializedCall(ESDSpecializations spec) {
		if (spec == ESDSpecializations::createStaticData) CreateStaticData<E>();
		else if (spec == ESDSpecializations::saveStaticData) SaveStaticData<E>();
	};

	template<FSD::Types E>
	static void CreateStaticData() {
		FSDTypeData<E> instanceStruct;
		std::vector<FSDInstanceProp> instanceProps = GetPropFromString();
		STDTypeAction::TypeAction(instanceStruct, instanceProps, ESDInstanceAction::loadingStaticData);
		UE_LOG(LogTemp, Warning, TEXT("---------> Adding instance data to map..."));
	};

	static std::vector<FSDInstanceProp> GetPropFromString() {
		FSDUtil::ExtractedChunks instancePropsChunks = FSDUtil::ExtractChunks(
			FSDUtil::FstringToStdString(FSDCore::CurrentInstanceString)
		);

		std::vector<FSDInstanceProp> instanceProps;

		// Parsing prop by prop
		for (std::string propChunk : instancePropsChunks.chunks) {
			FSDInstanceProp prop;
			UE_LOG(LogTemp, Warning, TEXT("---------> Parsing prop chunk: %s"), *FSDUtil::StdStringToFstring(propChunk));

			FSDUtil::ExtractedChunks propDescriptors = FSDUtil::ExtractChunks(propChunk, ',');

			int32 typeCode = FSDUtil::StdStringToInt32(propDescriptors.chunks[0]);
			prop.propValueType = static_cast<FSD::ValueTypes>(typeCode);
			prop.propName = propDescriptors.chunks[1];

			if (propDescriptors.count == 3) { // simple data
				prop.propValues.push_back(propDescriptors.chunks[2]);
			}
			else { // array
				prop.isArray = true;
				UE_LOG(LogTemp, Warning, TEXT("---------> About to write values for property containing array."));
				for (int i = 0; (i + 3) < propDescriptors.chunks.size(); ++i) {
					UE_LOG(LogTemp, Warning, TEXT("---------> Writing array value into Static Data instance property: %s"), *FSDUtil::StdStringToFstring(propDescriptors.chunks[i + 3]));
					prop.propValues.push_back(propDescriptors.chunks[i + 3]);
				}
			}

			instanceProps.push_back(prop);
		}

		return instanceProps;
	};

	template<FSD::Types E>
	static void SaveStaticData() {
		TMap<int32, FSDTypeData<E>> typeData = FSDSGetTypeData::GetTypeData<E>();

		FString line;
		for (auto& tuple : typeData)
		{
			FSDTypeData<E>& inst = tuple.Value;
			line = FSDUtil::StdStringToFstring(FEncodeInstanceData<E>(inst));
			FSDCore::StaticDataFString.Append(line);
		}
	};

	template<FSD::Types E>
	static std::string FEncodeInstanceData(FSDTypeData<E>& inst) {
		std::vector<FSDInstanceProp> instProps;
		STDTypeAction::TypeAction(inst, instProps, ESDInstanceAction::savingStaticData);

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
	};
};