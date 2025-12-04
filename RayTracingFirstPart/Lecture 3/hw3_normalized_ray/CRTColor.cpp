#include "CRTColor.h"

const int CRTColor::maxColorComponent = 255;

int CRTColor::generateRandomColorChannel()
{
    // the number is created using <random> library  
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, CRTColor::maxColorComponent);
    return dist(gen);
}

void CRTColor::changeColorChannel(char channel)
{
    switch (channel)
    {
    case 'r':
        redComponent = generateRandomColorChannel();
        break;
    case 'g':
        greenComponent = generateRandomColorChannel();
        break;
    case 'b':
        greenComponent = generateRandomColorChannel();
        break;
    default:
        break;
    }
}

std::ostream& operator<<(std::ostream& os, const CRTColor& color)
{
    return os << color.redComponent << " " << color.greenComponent << " " << color.blueComponent << " \t";
}

CRTColor::CRTColor(int red, int green, int blue) : redComponent(red), greenComponent(green), blueComponent(blue)
{
}

CRTColor CRTColor::generateRandomColor()
{
    int redComponent = generateRandomColorChannel();
    int greenComponent = generateRandomColorChannel();
    int blueComponent = generateRandomColorChannel();

    return CRTColor(redComponent, greenComponent, blueComponent);
}


