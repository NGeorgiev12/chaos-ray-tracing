#pragma once
#include <iostream>
#include <vector>

namespace Helper
{
	constexpr float EPSILON = 1e-5f;
	constexpr int UNIT_VEC_LENGTH = 1;
	constexpr int TRIANGLE_VERTICES = 3;
	constexpr int ROWS_COUNT = 3;
	constexpr int COLS_COUNT = 3;

	struct Point 
	{
		float x;
		float y;
		float z;
	};

	struct Grid 
	{
		int imageWidth;
		int imageHeight;
	};

	struct Pixel 
	{
		int x;
		int y;
	};

	bool areEqualFloats(float left, float right);
	bool operator==(const Point& lhs, const Point& rhs);
};

