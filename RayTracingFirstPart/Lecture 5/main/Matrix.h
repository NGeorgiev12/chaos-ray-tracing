#pragma once
#include <iostream>
#include <vector>

static constexpr int MAX_ROWS_COUNT = 3;
static constexpr int MAX_COLS_COUNT = 3;

class Matrix
{
public:

	Matrix() = default;
	Matrix(int rows, int cols, const std::vector<std::vector<float>>& numbers);

	int getRows() const;
	int getCols() const;
	const std::vector<std::vector<float>>& getNumbers() const;

private:
	int rows = 0;
	int cols = 0;
	std::vector<std::vector<float>> numbers;
};

float calculateDeterminant(const Matrix& mtx);

