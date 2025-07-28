// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "algorithms/CRTRenderer.h"

void task1() {

    const char SCENE_FILE[] = "scene/scene0.crtscene";
    const char OUTPUT_FILE[] = "output1.ppm";

    CRTRenderer renderer(SCENE_FILE, RenderType::BUCKETS, TraceType::AABB);
    renderer.renderScene(OUTPUT_FILE);
}
int main()
{
    task1();
}
