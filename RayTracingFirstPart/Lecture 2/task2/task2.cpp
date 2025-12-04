// task2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Circle.h"

constexpr int IMAGE_WIDTH = 1920;
constexpr int IMAGE_HEIGHT = 1080;

void constructCircleImage(int imageWidth, int imageHeight)
{
    std::ofstream ppmFileStream("task2_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << Color::maxColorComponent << "\n";
    
    int circleCenterWidth = IMAGE_WIDTH / 2;
    int circleCenterHeight = IMAGE_HEIGHT / 2;
    Circle circle(circleCenterWidth, circleCenterHeight);
    Color backgroundColor(Color::maxColorComponent, Color::maxColorComponent, Color::maxColorComponent);

    for (int i = 0; i < imageHeight; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            Point currentPixel { j, i };
            if (circle.isPointInCircle(currentPixel))
            {
                ppmFileStream << circle.getColor();
            }
            else
            {
                ppmFileStream << backgroundColor;
            }
        }
        ppmFileStream << "\n";
    }
    ppmFileStream.close();
}

int main()
{
    constructCircleImage(IMAGE_WIDTH, IMAGE_HEIGHT);
}