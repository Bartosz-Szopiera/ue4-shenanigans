#pragma once

#include "SDGetInstance.h"

template<FSD::Types T>
class FSDProvider {
public:
	void InitializeSD(int32 id) {
		SD = FSDGetInstance::GetTypeInstanceData<T>(id);
	};

	FSDTypeData<T> SD;
};
