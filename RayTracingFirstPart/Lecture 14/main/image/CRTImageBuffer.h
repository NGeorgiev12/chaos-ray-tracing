#pragma once
#include <fstream>
#include "CRTColor.h"

class CRTImageBuffer
{
public:

	CRTImageBuffer() = default;
	CRTImageBuffer(int width, int height);
	CRTImageBuffer(const Grid& resolution);

	const Grid& getResolution() const;
	const std::vector<CRTColor>& getPixels() const;
	const CRTColor& getPixel(int curWidth, int curHeight) const;
	void setPixel(int curWidth, int curHeight, CRTColor&& color);
	void save(const std::string& fileName, const Grid& grid) const;

private:

	Grid resolution;
	std::vector<CRTColor> pixels;

	void writeHeader(std::ofstream& ofs, const Grid& grid) const;

};

