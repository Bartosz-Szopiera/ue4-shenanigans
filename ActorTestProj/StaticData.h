// 2020 Purple Pill Productions.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "StaticData.generated.h"

USTRUCT()
struct FTypeDataBasic
{
    GENERATED_BODY()

    uint32 id;

    friend uint32 GetTypeHash(const FTypeDataBasic& MyClass)
    {
        uint32 HashCode = MyClass.id;
        return HashCode;
    }
};

struct FType1Data : public FTypeDataBasic
{
    int32 id;

    int32 prop1;
    int32 prop2;
};

struct FType2Data : public FTypeDataBasic
{
    int32 id;

    FString prop1;
    FString prop2;
};

enum class EStaticDataTypes {
    type1,
    type2,
};

struct FStaticData {
    TSet<FType1Data> type1;
    TSet<FType2Data> type2;
};
