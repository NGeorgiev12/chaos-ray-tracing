#pragma once
#include "Color.h"

struct Point
{
	int x = 0;
	int y = 0;

	int getDistanceToPoint(const Point& other) const
	{
		return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
	}
};

class Circle
{
	int radius = 0;
	Point circleCenter;
	Color color;
	 
public:

	Circle(int x, int y);
	bool isPointInCircle(const Point& point) const;
	const Color& getColor() const;
};

