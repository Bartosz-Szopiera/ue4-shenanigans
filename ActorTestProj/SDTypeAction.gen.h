#pragma once
/**
 * GENERATED FILE
 */
#include "SDCore.h"
#include "SDPropAction.h"
class STDTypeAction {
public:
	typedef std::vector<FSDInstanceProp>& P; // Vector of instance properties
	typedef ESDInstanceAction A; // Action to perform
	static void TypeAction(FSDTypeData<FSD::Types::type1>& i /* i - instance */, P p, A a) {
		STDPropertyAction::PA(i.id, "id", p, a);
		STDPropertyAction::PA(i.prop1, "prop1", p, a);
		STDPropertyAction::PA(i.prop2, "prop2", p, a);
		if (a == A::loadingStaticData) FSDCore::StaticData.type1.Add(i.id, i);
	};
	static void TypeAction(FSDTypeData<FSD::Types::type2>& i, P p, A a) {
		STDPropertyAction::PA(i.id, "id", p, a);
		STDPropertyAction::PA(i.prop1, "prop1", p, a);
		STDPropertyAction::PA(i.prop2, "prop2", p, a);
		STDPropertyAction::PA(i.prop3, "prop3", p, a);
		STDPropertyAction::PA(i.prop4, "prop4", p, a);
		STDPropertyAction::PA(i.prop5, "prop5", p, a);
		STDPropertyAction::PA(i.prop6, "prop6", p, a);
		STDPropertyAction::PA(i.prop7, "prop7", p, a);
		STDPropertyAction::PA(i.prop8, "prop8", p, a);
		if (a == A::loadingStaticData) FSDCore::StaticData.type2.Add(i.id, i);
	};
};
