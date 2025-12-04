#include "Helper.h"

bool Helper::areEqualFloats(float left, float right)
{
	return std::abs(left - right) < EPSILON;
}

bool Helper::operator==(const Point& lhs, const Point& rhs)
{
	return areEqualFloats(lhs.x, rhs.x) && areEqualFloats(lhs.y, rhs.y) && areEqualFloats(lhs.z, rhs.z);
}

float Helper::degreesToRadians(float degrees)
{
	return degrees * (PI / PI_DEGREES);
}


