#pragma once
#include "../math/CRTVector.h"
#include <float.h>

class CRTAxisAlignedBB
{
public:

	CRTAxisAlignedBB() = default;
	CRTAxisAlignedBB(const CRTVector& lowerBound, const CRTVector& upperBound);
	CRTAxisAlignedBB(CRTVector&& lowerBound, CRTVector&& upperBound);

	const CRTVector& getLowerBound() const;
	const CRTVector& getUpperBound() const;

	void includePoint(const CRTVector& point);

private:
	
	CRTVector lowerBound{ FLT_MAX, FLT_MAX, FLT_MAX };
	CRTVector upperBound{ FLT_MIN, FLT_MIN, FLT_MIN };
};

