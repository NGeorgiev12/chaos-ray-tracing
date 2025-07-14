#include "Matrix.h"
#include <assert.h>
#include "Helper.h"
using namespace Helper;

Matrix::Matrix(int rows, int cols, const std::vector<std::vector<float>>& numbers) : rows(rows),
cols(cols), numbers(numbers)
{
}

int Matrix::getRows() const
{
	return rows;
}

int Matrix::getCols() const
{
	return cols;
}

const std::vector<std::vector<float>>& Matrix::getNumbers() const
{
	return numbers;
}

float calculateDeterminant(const Matrix& mtx)
{
	assert(mtx.getRows() == mtx.getCols());

	float firstElem = mtx.getNumbers()[0][0];
	if (mtx.getRows() == 1) {
		return firstElem;
	}

	if (mtx.getRows() == 2) {
		return (mtx.getNumbers()[0][0] * mtx.getNumbers()[1][1])
			- (mtx.getNumbers()[1][0] * mtx.getNumbers()[0][1]);
	}

	float det = 0.0f;

	for (int i = 0; i < mtx.getRows(); i++) {

		int sign = (i % 2 == 0) ? 1 : -1;

		auto subMatrix = createSubmatrix(mtx, 0, i);
		float currentNumber = mtx.getNumbers()[0][i];

		det += sign * currentNumber * calculateDeterminant(Matrix(mtx.getRows() - 1, mtx.getCols() - 1, subMatrix));
	}
	
	return det;
}
