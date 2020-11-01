
#pragma once

#include "StaticData.h"
#include "StaticDataGetTypeData.gen.h"

class FSDGetInstance {
public:
	template<FSD::Types E>
	static FSDTypeData<E>& GetTypeInstanceData(int32 instanceId) {
		TMap<int32, FSDTypeData<E>> typeData = FSDSGetTypeData::GetTypeData<E>();
		bool hasInstance = typeData.Contains(instanceId);
		if (!hasInstance) {
			UE_LOG(LogTemp, Warning, TEXT("---------> There was no item"));
		}
		return typeData[instanceId];
	};
};