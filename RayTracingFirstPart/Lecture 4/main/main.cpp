#include <iostream>
#include "CRTriangle.h"

void task2() {

    CRTVector A1(3.5f, 0.0f, 0.0f);
    CRTVector B1(1.75f, 3.5f, 0.0f);
    CRTVector result1 = crossProduct(A1, B1);
    std::cout << "Cross Product A x B (1): (" << result1 << ")\n";

    CRTVector A2(3.0f, -3.0f, 1.0f);
    CRTVector B2(4.0f, 9.0f, 3.0f);
    CRTVector result2 = crossProduct(A2, B2);
    std::cout << "Cross Product A x B (2): (" << result2 << ")\n";


    CRTriangle triangle1(A2, B2);
    float area = triangle1.getArea() * 2.0f;
    float areaSame = result2.getLength();
    std::cout << "Area:" << area << " " << areaSame << "\n";

    //CRTVector A4 = A2;
    //CRTVector B4(-12.0f, 12.0f, -4.0f);
    //CRTriangle triangle2(A4, B4);
    //float area2 = triangle1.getArea() * 2.0f;
    //float areaSame2 = result2.getLength();
    //std::cout << "Area:" << area2 << " " << areaSame2 << "\n";
}

void task3() {

    Point A1 = { -1.75f, -1.75f, -3.0f };
    Point B1 = { 1.75f, -1.75f, -3.0f };
    Point C1 = { 0.0f,   1.75f, -3.0f };
    CRTriangle triangle1(A1, B1, C1);
    CRTVector normal1 = triangle1.normalVector();
    std::cout << "Area:" << triangle1.getArea() << ", normal vector: (" << normal1 << ")\n";

    Point A2 = { 0.0f,  0.0f, -1.0f };
    Point B2 = { 1.0f,  0.0f,  1.0f };
    Point C2 = { -1.0f,  0.0f,  1.0f };
    CRTriangle triangle2(A2, B2, C2);
    CRTVector normal2 = triangle2.normalVector();
    std::cout << "Area:" << triangle2.getArea() << ", normal vector: (" << normal2 << ")\n";

    Point A3 = { 0.56f,   1.11f,   1.23f };
    Point B3 = { 0.44f,  -2.368f, -0.54f };
    Point C3 = { -1.56f,   0.15f,  -1.92f };
    CRTriangle triangle3(A3, B3, C3);
    CRTVector normal3 = triangle3.normalVector();
    std::cout << "Area:" << triangle3.getArea() << ", normal vector: (" << normal3 << ")\n";
}

int main()
{
    task2();
    task3();
}