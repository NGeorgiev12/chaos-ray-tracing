#include "Helper.h"

bool Helper::areEqualFloats(float left, float right)
{
	return std::abs(left - right) < EPSILON;
}

bool Helper::operator==(const Point& lhs, const Point& rhs)
{
	return areEqualFloats(lhs.x, rhs.x) && areEqualFloats(lhs.y, rhs.y) && areEqualFloats(lhs.z, rhs.z);
}

std::vector<std::vector<float>> Helper::createSubmatrix(const Matrix& mtx, int excludeRow, int excludeCol)
{
	std::vector<std::vector<float>> subMtx;

	for (int i = 0; i < mtx.getRows(); i++) {

		if (i == excludeRow) {
			continue;
		}

		std::vector<float> row;

		for (int j = 0; j < mtx.getCols(); j++) {

			if (j == excludeCol) {
				continue;
			}

			row.push_back(mtx.getNumbers()[i][j]);
		}

		subMtx.push_back(row);
	}

	return subMtx;
}
