// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "algorithms/CRTRenderer.h"

void task1() {

    const char SCENE_FILE[] = "scene/scene0.crtscene";
    const char OUTPUT_FILE[] = "output0.ppm";

    CRTRenderer renderer(SCENE_FILE, RenderType::BUCKETS, TraceType::KD);
    renderer.renderScene(OUTPUT_FILE);
}

void task2() {

    const char SCENE_FILE[] = "scene/scene1.crtscene";
    const char OUTPUT_FILE[] = "output";

    CRTRenderer renderer(SCENE_FILE, RenderType::BUCKETS, TraceType::KD);
    renderer.renderAnimation(OUTPUT_FILE);
}

int main()
{
    task2();
}

