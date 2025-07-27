#include "BitMapBuffer.h"

BitMapBuffer::BitMapBuffer(const std::string& filePath)
{
    stbi_set_flip_vertically_on_load(1);

    int width = 0, height = 0, channels = 0;
    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    assert(image != nullptr && "Bitmap data is null!");

    this->resolution = { width, height };

    this->channels = channels;

    size_t dataSize = static_cast<size_t>(width) * height * channels;
    data.assign(image, image + dataSize);

    stbi_image_free(image);
}

const Grid& BitMapBuffer::getResolution() const
{
    return resolution;
}

const std::vector<unsigned char>& BitMapBuffer::getData() const
{
    return data;
}

int BitMapBuffer::getChannels() const
{
    return channels;
}

CRTVector BitMapBuffer::loadBitmapVector(int rowIdx, int colIdx) const
{
    assert(rowIdx < resolution.imageHeight && colIdx < resolution.imageWidth, "Pixel coordinates out of bounds");
    
    size_t index = (static_cast<size_t>(rowIdx) * resolution.imageWidth + colIdx) * channels;

    float red = data[index] / 255.f;
    float green = data[index + 1] / 255.f;
    float blue = data[index + 2] / 255.f;

    return CRTVector(red, green, blue);
}
