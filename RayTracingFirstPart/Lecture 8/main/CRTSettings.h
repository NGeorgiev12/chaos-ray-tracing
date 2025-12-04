#pragma once
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "Helper.h"
#include "CRTCamera.h"
#include "CRTMesh.h"
using namespace rapidjson;
using namespace Helper;

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

