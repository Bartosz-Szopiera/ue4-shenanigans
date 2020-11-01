#pragma once
#include "TypesExtended.gen.h"
#include "Types.gen.h"
//USTRUCT(BlueprintType)
struct FStaticData {
	//GENERATED_BODY()
	bool dataIsReady;
	TMap<int32, FSDTypeData<FSD::Types::type1>> type1;
	TMap<int32, FSDTypeData<FSD::Types::type2>> type2;
};