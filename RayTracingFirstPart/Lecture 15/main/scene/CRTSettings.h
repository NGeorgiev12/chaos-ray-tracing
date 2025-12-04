#pragma once
#include <string>
#include "../scene/CRTCamera.h"
#include "../scene/CRTMesh.h"

class CRTSettings  
{  
public:  

	CRTSettings();
	CRTSettings(const CRTVector& backgroundColor, const Grid& imageResolution, int bucketsCount);  
	CRTSettings(CRTVector&& backgroundColor, Grid&& imageResolution, int bucketsCount);

	const CRTVector& getBackgroundColor() const;  
	const Grid& getImageResolution() const;
	int getBucketsCount() const;
	bool getGI() const;
	int getGICount() const;

	void setBackgroundColor(CRTVector&& backgroundColor);
	void setImageResolution(Grid&& resolution);
	void setBucketsCount(int bucketsCount);
	void setGI(bool isGIOn);
	void setCountGI();
	void print() const;
private:  

	CRTVector backgroundColor;  
	Grid imageResolution;
	int bucketsCount = 0;
	bool isGIOn = false;
	int countGI = 0;
};

