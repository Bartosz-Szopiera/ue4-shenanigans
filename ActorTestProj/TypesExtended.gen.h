#pragma once
#include "Types.gen.h"
struct FSDTypeBase {
	int32 id;
	bool meta_isBound;
};
template<FSD::Types E>
struct FSDTypeData {
public:
	friend int32 GetTypeHash(const FSDTypeData<E>& myStruct) {
		return myStruct.id;
	};

	friend bool operator==(const FSDTypeData<E>& LHS, const FSDTypeData<E>& RHS)
	{
		return LHS.id == RHS.id;
	};
};
template<> struct FSDTypeData<FSD::Types::type1> : public FSDTypeBase, public FSD::FType1Data {};
template<> struct FSDTypeData<FSD::Types::type2> : public FSDTypeBase, public FSD::FType2Data {};