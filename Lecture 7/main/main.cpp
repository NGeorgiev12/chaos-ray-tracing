#include <iostream>  
#include "CRTRenderer.h"

const char TASK1[] = "scene0.crtscene";
const char TASK2[] = "scene1.crtscene";
const char TASK3[] = "scene2.crtscene";
const char TASK4[] = "scene3.crtscene";
const char TASK5[] = "scene4.crtscene";


int main()  {
	
	CRTRenderer renderer(TASK5);
	renderer.renderScene();
}
