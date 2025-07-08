#include "Color.h"

const int Color::maxColorComponent = 255;

int Color::generateRandomColorChannel()
{
    // the number is created using <random> library  
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, Color::maxColorComponent);
    return dist(gen);
}

void Color::changeColorChannel(char channel)
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

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    return os << color.redComponent << " " << color.greenComponent << " " << color.blueComponent << " \t";
}

Color::Color(int red, int green, int blue) : redComponent(red), greenComponent(green), blueComponent(blue)
{
}

Color Color::generateRandomColor()
{
    int redComponent = generateRandomColorChannel();
    int greenComponent = generateRandomColorChannel();
    int blueComponent = generateRandomColorChannel();

    return Color(redComponent, greenComponent, blueComponent);
}


