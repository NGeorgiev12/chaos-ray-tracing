// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CRTRenderer.h"

void task1() {
    
	const char SCENE_FILE[] = "scene/scene0.crtscene";
	const char OUTPUT_FILE[] = "output1.ppm";
	CRTRenderer renderer(SCENE_FILE);
	renderer.renderScene(OUTPUT_FILE);
}

void task2() {
	
	const char SCENE_FILE[] = "scene/scene1.crtscene";
	const char OUTPUT_FILE[] = "output10.ppm";
	CRTRenderer renderer(SCENE_FILE);
	renderer.renderScene(OUTPUT_FILE);
}

void task3() {
	
	const char SCENE_FILE[] = "scene/scene2.crtscene";
	const char OUTPUT_FILE[] = "output3.ppm";
	CRTRenderer renderer(SCENE_FILE);
	renderer.renderScene(OUTPUT_FILE);
}

void task4() {
	
	const char SCENE_FILE[] = "scene/scene3.crtscene";
	const char OUTPUT_FILE[] = "output4.ppm";
	CRTRenderer renderer(SCENE_FILE);
	renderer.renderScene(OUTPUT_FILE);
}

int main()
{
	task2();
}