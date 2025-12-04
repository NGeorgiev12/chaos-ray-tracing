#pragma once
#include <iostream>
#include <vector>
#include "Matrix.h"

namespace Helper
{
	constexpr float EPSILON = 1e-5f;
	constexpr int UNIT_VEC_LENGTH = 1;
	constexpr int TRIANGLE_VERTICES = 3;

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

	bool areEqualFloats(float left, float right);
	bool operator==(const Point& lhs, const Point& rhs);
	std::vector<std::vector<float>> createSubmatrix(const Matrix& mtx, int excludeRow, int excludeCol);

};

