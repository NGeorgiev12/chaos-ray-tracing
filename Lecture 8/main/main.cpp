#include <iostream>
#include "CRTRenderer.h"

void task1() {

	const char sceneFileName[] = "scene0.crtscene";
	const char outputFileName[] = "output1.ppm";
	CRTRenderer renderer(sceneFileName);
	renderer.renderScene(outputFileName);
}

void task2() {

	const char sceneFileName[] = "scene1.crtscene";
	const char outputFileName[] = "output2.ppm";
	CRTRenderer renderer(sceneFileName);
	renderer.renderScene(outputFileName);
}

void task3() {

	const char sceneFileName[] = "scene2.crtscene";
	const char outputFileName[] = "output3.ppm";
	CRTRenderer renderer(sceneFileName);
	renderer.renderScene(outputFileName);
}

void task4() {

	const char sceneFileName[] = "scene3.crtscene";
	const char outputFileName[] = "output4.ppm";
	CRTRenderer renderer(sceneFileName);
	renderer.renderScene(outputFileName);
}
int main()
{
	task4();
}
