#pragma once
#include <string>
#include "../scene/CRTCamera.h"
#include "../scene/CRTMesh.h"

class CRTSettings  
{  
public:  

	CRTSettings();
	CRTSettings(const CRTVector& backgroundColor, const Grid& imageResolution);  
	CRTSettings(CRTVector&& backgroundColor, Grid&& imageResolution);  

	const CRTVector& getBackgroundColor() const;  
	const Grid& getImageResolution() const;  
private:  

	CRTVector backgroundColor;  
	Grid imageResolution;  

};

