#include "CRTImageBuffer.h"

CRTImageBuffer::CRTImageBuffer(int width, int height)
    : resolution{ width, height }, pixels(width* height)
{
}

CRTImageBuffer::CRTImageBuffer(const Grid& resolution)
    : resolution(resolution), pixels(resolution.imageWidth* resolution.imageHeight)
{
}

const Grid& CRTImageBuffer::getResolution() const 
{
    return resolution;
}

const std::vector<CRTColor>& CRTImageBuffer::getPixels() const
{
    return pixels;
}

const CRTColor& CRTImageBuffer::getPixel(int curWidth, int curHeight) const
{
    size_t index = static_cast<size_t>(curHeight) * resolution.imageWidth + curWidth;
    return pixels[index];
}

void CRTImageBuffer::setPixel(int curWidth, int curHeight, CRTColor&& color)
{
    size_t index = static_cast<size_t>(curHeight) * resolution.imageWidth + curWidth;
    pixels[index] = std::move(color);
}

void CRTImageBuffer::save(const std::string& fileName, const Grid& grid) const
{
    std::ofstream ofs(fileName, std::ios::out | std::ios::binary);
    assert(ofs.is_open() && "Failed to open output file for writing");

    writeHeader(ofs, grid);

    for (size_t i = 0; i < grid.imageHeight; i++)
    {
        for (size_t j = 0; j < grid.imageWidth; j++)
        {
            ofs << pixels[i * grid.imageWidth + j] << "\t";
        }
        ofs << "\n";
    }

    ofs.close();
}

void CRTImageBuffer::writeHeader(std::ofstream& ofs, const Grid& grid) const
{
    ofs << "P3\n";
    ofs << grid.imageWidth << " " << grid.imageHeight << "\n";
    ofs << MAX_COLOR_COMPONENT << "\n";
}
