// task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "Color.h"

constexpr int IMAGE_WIDTH = 1920;
constexpr int IMAGE_HEIGHT = 1080;
constexpr int ROWS_RECTANGLES_COUNT = 7;
constexpr int COLS_RECTANGLES_COUNT = 12;
const char channels[] = { 'r', 'g', 'b' };

struct RectanglesGrid
{
    int imageWidth = 0;
    int imageHeight = 0;
    int rowsRectanglesCount = 0;
    int colsRectanglesCount = 0;
};

void generateRandomRectColors(std::vector<Color>& colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = Color::generateRandomColor();
    }
}

void constructImageDividedByRectangles(const RectanglesGrid& grid)
{
    std::ofstream ppmFileStream("task1_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << grid.imageWidth << " " << grid.imageHeight << "\n";
    ppmFileStream << Color::maxColorComponent << "\n";

    int rectWidth = grid.imageWidth / grid.colsRectanglesCount;
    int rectHeight = grid.imageHeight / grid.rowsRectanglesCount;

    std::vector<Color> rectColors(grid.colsRectanglesCount);
    generateRandomRectColors(rectColors);
    char currentChannel;

    for (int i = 0; i < grid.imageHeight; i++) 
    {

        if ((i % rectHeight) == 0)
        {
            generateRandomRectColors(rectColors);
            currentChannel = channels[rand() % 3];
        }

        for (int j = 0; j < grid.imageWidth; j++) {

            int rowIdx = (i / rectHeight);
            int colIdx = (j / rectWidth);

            int colorIndex = (j / rectWidth);
            Color rectColor = rectColors[colorIndex];
            rectColor.changeColorChannel(currentChannel);
            ppmFileStream << rectColor;
        }
        ppmFileStream << "\n";
    }
    ppmFileStream.close();
}

int main()
{
    RectanglesGrid grid{ IMAGE_WIDTH, IMAGE_HEIGHT, ROWS_RECTANGLES_COUNT, COLS_RECTANGLES_COUNT };
    constructImageDividedByRectangles(grid);

}
