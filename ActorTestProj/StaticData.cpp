// 2020 Purple Pill Productions.

#include "StaticData.h"

void main() {
	FStaticData StaticData;

	FType1Data entity1type1;
	entity1type1.id = 1;
	entity1type1.prop1 = 11;
	entity1type1.prop2 = 12;

	StaticData.type1.Add(entity1type1);

	//FType1Data entity2type1;
	//entity1type1.id = 12321111;
	//entity1type1.prop1 = 111;
	//entity1type1.prop2 = 112;

	//StaticData.type1.Add(entity2type1);

	//FType2Data entity1type2;
	//entity1type1.id = 112222345555;
	//entity1type1.prop1 = 21;
	//entity1type1.prop2 = 22;

	//StaticData.type2.Add(entity1type2);

	//FType2Data entity2type2;
	//entity1type1.id = 11333345555;
	//entity1type1.prop1 = 221;
	//entity1type1.prop2 = 222;

	//StaticData.type2.Add(entity2type2);
}
