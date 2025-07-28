#include "CRTAxisAlignedBB.h"

CRTAxisAlignedBB::CRTAxisAlignedBB(const CRTVector& lowerBound, const CRTVector& upperBound)
	: lowerBound(lowerBound), upperBound(upperBound)
{
}

CRTAxisAlignedBB::CRTAxisAlignedBB(CRTVector&& lowerBound, CRTVector&& upperBound)
	: lowerBound(std::move(lowerBound)), upperBound(std::move(upperBound))
{
}

const CRTVector& CRTAxisAlignedBB::getLowerBound() const
{
	return lowerBound;
}

const CRTVector& CRTAxisAlignedBB::getUpperBound() const
{
	return upperBound;
}

void CRTAxisAlignedBB::includePoint(const CRTVector& point)
{
	lowerBound = min(lowerBound, point);
	upperBound = max(upperBound, point);
}
