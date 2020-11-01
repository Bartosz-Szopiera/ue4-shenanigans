#pragma once

#include "SDCore.h"

class STDPropertyAction {
public:
	template<class T>
	// PropertyAction
	static void PA(T& instanceProperty, std::string propName, std::vector<FSDInstanceProp>& properties, ESDInstanceAction action) {
		if (action == ESDInstanceAction::loadingStaticData) {
			UE_LOG(LogTemp, Warning, TEXT("---------> Trying to write to instance prop: %s."), *FSDUtil::StdStringToFstring(propName));
			for (FSDInstanceProp prop : properties) {
				if (propName == prop.propName) {
					for (int32 i = 0; i < prop.propValues.size(); i++) {
						UE_LOG(LogTemp, Warning, TEXT("---------> Write to instance value: %s."), *FSDUtil::StdStringToFstring(prop.propValues[i]));
						FSDUtil::SetEntityValueFromStdString(instanceProperty, prop.propValues[i]);
					}
					return;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("---------> [!!!!] Prop of name: '%s' not found."), *FSDUtil::StdStringToFstring(propName));
		}
		else if (action == ESDInstanceAction::savingStaticData) {
			FSDInstanceProp prop;
			prop.propName = propName;
			prop.propValueType = FSDUtil::GetValueTypeFromType<T>();
			FSDUtil::ToStdStringAndAssign(instanceProperty, prop);

			properties.push_back(prop);
		}
	};
};
