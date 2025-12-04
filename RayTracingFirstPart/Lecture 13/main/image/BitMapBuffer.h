#pragma once
#include "../math/CRTVector.h"
#include "stbImage.h"

class BitMapBuffer
{
public:

	BitMapBuffer() = default;
	BitMapBuffer(const std::string& filePath);

	const Grid& getResolution() const;
	const std::vector<unsigned char>& getData() const;
	int getChannels() const;

	CRTVector loadBitmapVector(int x, int y) const;

private:

	Grid resolution;
	int channels;
	std::vector<unsigned char> data;
};

