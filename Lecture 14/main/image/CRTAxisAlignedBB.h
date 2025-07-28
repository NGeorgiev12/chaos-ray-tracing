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

	void setLowerBoundComponent(int index, float value);
	void setUpperBoundComponent(int index, float value);

	void includePoint(const CRTVector& point);
	void includeBox(const CRTAxisAlignedBB& box);
	std::pair< CRTAxisAlignedBB, CRTAxisAlignedBB> split(int axis) const;

private:
	
	CRTVector lowerBound{ FLT_MAX, FLT_MAX, FLT_MAX };
	CRTVector upperBound{ FLT_MIN, FLT_MIN, FLT_MIN };
};

bool intersects(const CRTAxisAlignedBB& lhs, const CRTAxisAlignedBB& rhs);

