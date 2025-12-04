
#include <iostream>
#include "algorithms/CRTRenderer.h"

void task1()
{

    const char SCENE_FILE[] = "scene/scene0.crtscene";
    const char OUTPUT_FILE[] = "output0.ppm";

    CRTRenderer renderer(SCENE_FILE, RenderType::BUCKETS, TraceType::KD);
    renderer.renderScene(OUTPUT_FILE);
}

void task2()
{

    const char SCENE_FILE[] = "scene/scene1.crtscene";
    const char OUTPUT_FILE[] = "output1.ppm";

    CRTRenderer renderer(SCENE_FILE, RenderType::BUCKETS, TraceType::KD);
    renderer.renderScene(OUTPUT_FILE);
}

int main()
{
    task2();
}

