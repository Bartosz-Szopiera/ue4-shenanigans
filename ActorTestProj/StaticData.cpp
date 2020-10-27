// 2020 Purple Pill Productions.

#include "StaticData.h"

/*
* Static Data Manager
*/
class FSDManager {
	/*FSDManager() {
		initStaticData();
	};*/

private:
	/*void initStaticData() {
		if (!StaticData.dataIsSet) {
			FSetStaticData();
		}
	};*/

public:
	// Does it need to be static after all?
	static FStaticData StaticData;

	template<ESDTypes E>
	static TMap<uint32, FTypeData<E>>& getTypeData(ESDTypes dataType) {
		switch (dataType)
		{
		case ESDTypes::type1:
			return StaticData.type1;
			break;
		case ESDTypes::type2:
			return StaticData.type2;
			break;
		}
	};

	template<ESDTypes E>
	static FTypeData<E>& getTypeInstanceData(ESDTypes dataType, uint32 instanceId) {
		TMap<uint32, FTypeData<E>>& typeData = getTypeData(dataType);
		return typeData[instanceId];
	};
};

FStaticData FSetStaticData() {
	// type1
	FTypeData<ESDTypes::type1> entity1type1;
	entity1type1.id = 1;
	entity1type1.prop1 = 11;
	entity1type1.prop2 = 12;

	FSDManager::StaticData.type1.Add(entity1type1.id, entity1type1);

	// type2
	FTypeData<ESDTypes::type2> entity1type2;
	entity1type1.id = 21;
	entity1type1.prop1 = 21;
	entity1type1.prop2 = 22;

	FSDManager::StaticData.type2.Add(entity1type2.id, entity1type2);

	// Wrapping up
	FSDManager::StaticData.dataIsSet = true;
	return FSDManager::StaticData;
}

/**
 * Initializing static data
 */
FStaticData FSDManager::StaticData = FSetStaticData();
