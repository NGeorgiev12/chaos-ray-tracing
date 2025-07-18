#pragma once
#include <fstream>
#include <random>

class CRTColor
{
	int redComponent = 0;
	int greenComponent = 0;
	int blueComponent = 0;
	
public:

	static const int maxColorComponent;

	CRTColor() = default;
	CRTColor(int red, int green, int blue);

	void setRed(int newRed);
	void setGreen(int newGreen);
	void setBlue(int newBlue);

	friend std::ostream& operator<<(std::ostream& os, const CRTColor& color);
	static int generateRandomColorChannel();
	void changeColorChannel(char channel);
	static CRTColor generateRandomColor();
};

