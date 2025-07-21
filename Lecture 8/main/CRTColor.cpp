#include "CRTColor.h"
#include <assert.h>

const int CRTColor::maxColorComponent = 255;

static int floatToUint8(float value) {
    // Clamp value to [0.0f, 1.0f]
    value = std::max(0.0f, std::min(1.0f, value));
    return static_cast<int>(value * 255.0f);
}

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
        blueComponent = generateRandomColorChannel();
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

CRTColor::CRTColor(const CRTVector& vec)
{
	redComponent = floatToUint8(vec.getVertex().x);
	greenComponent = floatToUint8(vec.getVertex().y);
	blueComponent = floatToUint8(vec.getVertex().z);
}

CRTColor CRTColor::black() {  
    return CRTColor(0, 0, 0);  
}

int CRTColor::getRedComponent() const
{
    return redComponent;
}

int CRTColor::getGreenComponent() const
{
    return greenComponent;
}

int CRTColor::getBlueComponent() const
{
    return blueComponent;
}

void CRTColor::setRed(int newRed)
{
    assert(newRed >= 0 && newRed <= 255);
    redComponent = newRed;
}

void CRTColor::setGreen(int newGreen)
{
    assert(newGreen >= 0 && newGreen <= 255);
    greenComponent = newGreen;
}

void CRTColor::setBlue(int newBlue)
{
    assert(newBlue >= 0 && newBlue <= 255);
    blueComponent = newBlue;
}

CRTColor& CRTColor::operator+=(const CRTColor& other)
{
	redComponent = std::min(redComponent + other.redComponent, maxColorComponent);
	greenComponent = std::min(greenComponent + other.greenComponent, maxColorComponent);
	blueComponent = std::min(blueComponent + other.blueComponent, maxColorComponent);

    return *this;
}

CRTColor CRTColor::generateRandomColor()
{
    int redComponent = generateRandomColorChannel();
    int greenComponent = generateRandomColorChannel();
    int blueComponent = generateRandomColorChannel();

    return CRTColor(redComponent, greenComponent, blueComponent);
}

bool operator==(const CRTColor& lhs, const CRTColor& rhs)
{
    return lhs.redComponent == rhs.redComponent &&
           lhs.greenComponent == rhs.greenComponent &&
           lhs.blueComponent == rhs.blueComponent;
}

bool operator!=(const CRTColor& lhs, const CRTColor& rhs)
{
    return !(lhs == rhs);
}


