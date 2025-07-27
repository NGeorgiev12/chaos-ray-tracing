#include "CRTAlbedo.h"

CRTVector CRTAlbedo::albedo(const CRTexture& texture, const CRTIntersectionResult& result)
{
    float u = result.baryCoords.u;
    float v = result.baryCoords.v;
    float w = result.baryCoords.w;

    float xCoord = result.pUV.getComponent(0);
    float yCoord = result.pUV.getComponent(1);

    switch (texture.getType())
    {
        case TextureType::ALBEDO:
        {
            const AlbedoData& data = texture.getAlbedoData();
            return data.albedo;
        }
        case TextureType::EDGES:
        {
            const EdgesData& data = texture.getEdgesData();
            float edgeWidth = data.edgeWidth;

            if (u < edgeWidth || v < edgeWidth || w < edgeWidth) {
                return data.edgeColor;
            }

            return data.innerColor;
        }
        case TextureType::CHECKER:
        {
            const CheckerData& data = texture.getCheckerData();
            float squareSize = data.squareSize;

            unsigned int x = static_cast<int>(xCoord / squareSize);
            unsigned int y = static_cast<int>(yCoord / squareSize);

            bool isEven = (x + y) % 2 == 0;
            return isEven ? data.colorA : data.colorB;
        }
        case TextureType::BITMAP:
        {
            const BitmapData& data = texture.getBitmapData();
            const BitMapBuffer& buffer = data.buffer;

            unsigned int rowIdx = yCoord * buffer.getResolution().imageHeight;
            unsigned int colIdx = xCoord * buffer.getResolution().imageWidth;;

            return buffer.loadBitmapVector(rowIdx, colIdx);
        }
        case TextureType::INVALID:
        {
            assert(false);
        }
    }
}