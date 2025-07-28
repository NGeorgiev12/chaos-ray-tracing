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

void CRTAxisAlignedBB::setLowerBoundComponent(int index, float value)
{
	assert(index >= 0 && index < 3 && "Index is out of bounds");
	lowerBound.setComponent(index, value);
}

void CRTAxisAlignedBB::setUpperBoundComponent(int index, float value)
{
	assert(index >= 0 && index < 3 && "Index is out of bounds");
	upperBound.setComponent(index, value);
}

void CRTAxisAlignedBB::includePoint(const CRTVector& point)
{
	lowerBound = min(lowerBound, point);
	upperBound = max(upperBound, point);
}

void CRTAxisAlignedBB::includeBox(const CRTAxisAlignedBB& box)
{
	includePoint(box.getLowerBound());
	includePoint(box.getUpperBound());
}

std::pair<CRTAxisAlignedBB, CRTAxisAlignedBB> CRTAxisAlignedBB::split(int axis) const
{
	float middle = (lowerBound.getComponent(axis) + upperBound.getComponent(axis)) / 2.f;

	CRTAxisAlignedBB leftAABB = *this;
	CRTAxisAlignedBB rightAABB = *this;

	leftAABB.setUpperBoundComponent(axis, middle);
	rightAABB.setLowerBoundComponent(axis, middle);

	return { leftAABB, rightAABB };
}

bool intersects(const CRTAxisAlignedBB& lhs, const CRTAxisAlignedBB& rhs)
{
	const CRTVector& lhsLower = lhs.getLowerBound();
	const CRTVector& lhsUpper = lhs.getUpperBound();
	const CRTVector& rhsLower = rhs.getLowerBound();
	const CRTVector& rhsUpper = rhs.getUpperBound();

	return lhsLower.getComponent(0) >= rhsLower.getComponent(0) && lhsUpper.getComponent(0) <= rhsUpper.getComponent(0) &&
		lhsLower.getComponent(1) >= rhsLower.getComponent(1) && lhsUpper.getComponent(1) <= rhsUpper.getComponent(1) &&
		lhsLower.getComponent(2) >= rhsLower.getComponent(2) && lhsUpper.getComponent(2) <= rhsUpper.getComponent(2);

}
