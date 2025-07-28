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

	void setBackgroundColor(CRTVector&& backgroundColor);
	void setImageResolution(Grid&& resolution);
	void setBucketsCount(int bucketsCount);

	void print() const;
private:  

	CRTVector backgroundColor;  
	Grid imageResolution;
	int bucketsCount = 0;

};

