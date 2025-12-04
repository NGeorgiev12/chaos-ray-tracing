#include <iostream>
#include "CRTColor.h"
#include "CRTVector.h"

constexpr int IMAGE_WIDTH = 1920;
constexpr int IMAGE_HEIGHT = 1080;

struct Grid {

    int imageWidth;
    int imageHeight;

};

void constructImage(const Grid& grid) {

    std::ofstream ppmFs("output_image.ppm", std::ios::out | std::ios::binary);
    ppmFs << "P3\n";
    ppmFs << grid.imageWidth << " " << grid.imageHeight << "\n";
    ppmFs << CRTColor::maxColorComponent << "\n";

    for (int i = 0; i < grid.imageHeight; i++) {

        for (int j = 0; j < grid.imageWidth; j++) {

            float centreX = j + 0.5f;
            float centreY = i + 0.5f;

            centreX /= grid.imageWidth;
            centreY /= grid.imageHeight;

            centreX = (2.0f * centreX - 1.0f);
            centreY = (1.0f - 2.0f * centreY);

            centreX *= (float) (grid.imageWidth / grid.imageHeight);

            CRTVector rayDir(centreX, centreY, -1.0f);
            rayDir.normalize();

            float red = rayDir.getVertex().x * 255;
            float green = rayDir.getVertex().y * 255;
            float blue = rayDir.getVertex().z * -((red + green) / 2);

            CRTColor pixelColor((int)red, (int)green, (int)blue);
            ppmFs << pixelColor << "\t";
        }
        ppmFs << "\n";
    }

    ppmFs.close();
}

int main()
{
    Grid grid{ IMAGE_WIDTH, IMAGE_HEIGHT };
    constructImage(grid);
}