#pragma once  
#include <iostream>  
#include <vector>  

namespace Helper  
{
	constexpr float SHADOW_BIAS = 1e-2f;
	constexpr float EPSILON = 1e-5f;  
	constexpr int UNIT_VEC_LENGTH = 1;  
	constexpr int TRIANGLE_VERTICES = 3;  
	constexpr int ROWS_COUNT = 3;  
	constexpr int COLS_COUNT = 3;  
	constexpr float INITIAL_CAMERA_Z_DIR = -1.0f;  
	constexpr float PI = 3.14159f;  
	constexpr int PI_DEGREES = 180;
	constexpr int FLOATS_PER_VERTEX = 3;
	constexpr int MAX_RAY_DEPTH = 5;

	const char CRT_SCENE_SETTINGS[] = "settings";
	const char CRT_SCENE_BACKGROUND_COLOR[] = "background_color";
	const char CRT_SCENE_IMAGE_RESOLUTION[] = "image_settings";
	const char CRT_SCENE_IMAGE_RESOLUTION_WIDTH[] = "width";
	const char CRT_SCENE_IMAGE_RESOLUTION_HEIGHT[] = "height";
	const char CRT_SCENE_CAMERA[] = "camera";
	const char CRT_SCENE_CAMERA_MATRIX[] = "matrix";
	const char CRT_SCENE_CAMERA_POSITION[] = "position";
	const char CRT_SCENE_MATERIALS[] = "materials";
	const char CRT_SCENE_MATERIAL_TYPE[] = "type";
	const char CRT_SCENE_MATERIAL_ALBEDO[] = "albedo";
	const char CRT_SCENE_MATERIAL_SHADING[] = "smooth_shading";
	const char CRT_SCENE_LIGHTS[] = "lights";
	const char CRT_SCENE_LIGHT_INTENSITY[] = "intensity";
	const char CRT_SCENE_LIGHT_POSITION[] = "position";
	const char CRT_SCENE_OBJECTS[] = "objects";
	const char CRT_SCENE_MATERIAL_INDEX[] = "material_index";
	const char CRT_SCENE_VERTICES[] = "vertices";
	const char CRT_SCENE_TRIANGLES[] = "triangles";

	struct Point   
	{  
		float x;  
		float y;  
		float z;  
	};  

	struct Grid   
	{  
		int imageWidth;  
		int imageHeight;  
	};  

	struct Pixel   
	{  
		int x;  
		int y;  
	};

	struct BarycentricCoordinates  
	{  
		float u;  
		float v;
		float w;
	};

	bool areEqualFloats(float left, float right);  
	bool operator==(const Point& lhs, const Point& rhs);  
	float degreesToRadians(float degrees);  
};
