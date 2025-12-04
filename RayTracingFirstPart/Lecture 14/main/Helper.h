#pragma once  
#include <iostream>  
#include <vector>  
#include <cmath>

namespace Helper  
{
	constexpr int MAX_COLOR_COMPONENT = 255;
	constexpr int RED_COMPONENT = 0;
	constexpr int GREEN_COMPONENT = 1;
	constexpr int BLUE_COMPONENT = 2;
	constexpr float SHADOW_BIAS = 1e-2f;
	constexpr float REFLECTION_BIAS = 1e-2f;
	constexpr float REFRACTION_BIAS = 1e-2f;
	constexpr float EPSILON = 1e-5f;  
	constexpr int UNIT_VEC_LENGTH = 1;  
	constexpr int TRIANGLE_VERTICES = 3;  
	constexpr int ROWS_COUNT = 3;  
	constexpr int COLS_COUNT = 3;  
	constexpr float INITIAL_CAMERA_Z_DIR = -1.0f;  
	constexpr float PI = 3.14159f;  
	constexpr int PI_DEGREES = 180;
	constexpr int FLOATS_PER_VERTEX = 3;
	constexpr int MAX_KD_TREE_DEPTH = 12;
	constexpr int MAX_RAY_DEPTH = 5;
	constexpr float IOR_AIR = 1.0f;
	constexpr int AXIS_COUNT = 3;
	constexpr int MAX_BOX_TRIANGLE_COUNT = 4;
	constexpr int DEFAULT_BUCKETS_COUNT = 24;
	constexpr int FRAMES_PER_ANIMATION = 12;

	const char CRT_SCENE_SETTINGS[] = "settings";
	const char CRT_SCENE_BACKGROUND_COLOR[] = "background_color";
	const char CRT_SCENE_IMAGE_RESOLUTION[] = "image_settings";
	const char CRT_SCENE_IMAGE_RESOLUTION_WIDTH[] = "width";
	const char CRT_SCENE_IMAGE_RESOLUTION_HEIGHT[] = "height";
	const char CRT_SCENE_IMAGE_BUCKETS_COUNT[] = "bucket_size";
	const char CRT_SCENE_CAMERA[] = "camera";
	const char CRT_SCENE_CAMERA_MATRIX[] = "matrix";
	const char CRT_SCENE_CAMERA_POSITION[] = "position";
	const char CRT_SCENE_MATERIALS[] = "materials";
	const char CRT_SCENE_MATERIAL_TYPE[] = "type";
	const char CRT_SCENE_MATERIAL_ALBEDO[] = "albedo";
	const char CRT_SCENE_MATERIAL_IOR[] = "ior";
	const char CRT_SCENE_MATERIAL_SHADING[] = "smooth_shading";
	const char CRT_SCENE_LIGHTS[] = "lights";
	const char CRT_SCENE_LIGHT_INTENSITY[] = "intensity";
	const char CRT_SCENE_LIGHT_POSITION[] = "position";
	const char CRT_SCENE_OBJECTS[] = "objects";
	const char CRT_SCENE_MATERIAL_INDEX[] = "material_index";
	const char CRT_SCENE_VERTICES[] = "vertices";
	const char CRT_SCENE_TRIANGLES[] = "triangles";
	const char CRT_SCENE_TEXTURES[] = "textures";
	const char CRT_SCENE_TEXTURE_NAME[] = "name";
	const char CRT_SCENE_TEXTURE_TYPE[] = "type";
	const char CRT_SCENE_TEXTURE_ALBEDO[] = "albedo";
	const char CRT_SCENE_TEXTURE_EDGE_COLOR[] = "edge_color";
	const char CRT_SCENE_TEXTURE_INNER_COLOR[] = "inner_color";
	const char CRT_SCENE_TEXTURE_EDGE_WIDTH[] = "edge_width";
	const char CRT_SCENE_TEXTURE_COLOR_A[] = "color_A";
	const char CRT_SCENE_TEXTURE_COLOR_B[] = "color_B";
	const char CRT_SCENE_TEXTURE_SQUARE_SIZE[] = "square_size";
	const char CRT_SCENE_TEXTURE_FILEPATH[] = "file_path";
	const char CRT_SCENE_OBJECT_UVS[] = "uvs";

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
		int x = 0;  
		int y = 0;  
	};

	struct BarycentricCoordinates  
	{  
		float u;  
		float v;
		float w;
	};

	struct Interval
	{
		Pixel start;
		Pixel end;

		Interval() = default;
		Interval(const Pixel& start, const Pixel& end) : start(start), end(end) {}
	};

	bool areEqualFloats(float left, float right);  
	bool operator==(const Point& lhs, const Point& rhs);  
	float degreesToRadians(float degrees);
	float fresnelFormula(float dotProduct);
};
