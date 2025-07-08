#pragma once
#include <fstream>
#include <random>

class Color
{
	int redComponent = 0;
	int greenComponent = 0;
	int blueComponent = 0;
	
public:

	static const int maxColorComponent;

	Color() = default;
	Color(int red, int green, int blue);

	friend std::ostream& operator<<(std::ostream& os, const Color& color);
	static int generateRandomColorChannel();
	void changeColorChannel(char channel);
	static Color generateRandomColor();
};

