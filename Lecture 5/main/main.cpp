#include <iostream>
#include "CRTriangle.h"
#include "CameraRay.h"
#include "CRTColor.h"
#include <fstream>
#include <assert.h>

constexpr int IMAGE_WIDTH = 640;
constexpr int IMAGE_HEIGHT = 360;
constexpr float Z_DIR = -1.0f;

bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle) {

    const CRTVector& normal = triangle.normalVector();
    const CRTVector& v0 = triangle.getVertices()[0];
    const CRTVector& v1 = triangle.getVertices()[1];
    const CRTVector& v2 = triangle.getVertices()[2];

    CRTVector edge0 = v1 - v0;
    CRTVector edge1 = v2 - v1;
    CRTVector edge2 = v0 - v2;

    CRTVector toP0 = p - v0;
    CRTVector toP1 = p - v1;
    CRTVector toP2 = p - v2;

    bool checkE0 = dotProduct(normal, crossProduct(edge0, toP0)) > 0 ? true : false;
    bool checkE1 = dotProduct(normal, crossProduct(edge1, toP1)) > 0 ? true : false;
    bool checkE2 = dotProduct(normal, crossProduct(edge2, toP2)) > 0 ? true : false;

    return checkE0 && checkE1 && checkE2;
}

void constructTriangle(const Grid& grid, const CRTriangle& triangle) {

    const CRTVector& normal = triangle.normalVector();
    const CRTVector& v0 = triangle.getVertices()[0];

    std::ofstream ppmFs("output_image4.ppm", std::ios::out | std::ios::binary);
    ppmFs << "P3\n";
    ppmFs << grid.imageWidth << " " << grid.imageHeight << "\n";
    ppmFs << CRTColor::maxColorComponent << "\n";

    for (int i = 0; i < grid.imageHeight; i++) {

        for (int j = 0; j < grid.imageWidth; j++) {

            CRTVector cameraRay = CameraRay::generateCameraRay(grid, j, i, Z_DIR);

            float dotNormalCameraRay = dotProduct(normal, cameraRay);
            float dotNormalToVertex0 = dotProduct(normal, v0);

            if (areEqualFloats(dotNormalCameraRay, 0) || dotNormalToVertex0 >= 0.f) {
                continue;
            }

            float distanceToPlane = -dotNormalToVertex0;
            float projection = std::abs(dotNormalCameraRay);

            float t = distanceToPlane / projection;
            CRTVector intersectionPoint = t * cameraRay;
            CRTColor pixelColor;

            if (isPointInsideTriangle(intersectionPoint, triangle)) {

                pixelColor.setGreen(255);
                pixelColor.setBlue(255);
            }

            ppmFs << pixelColor << "\t";
        }
        ppmFs << "\n";
    }
}

// functions for the two triangles 
void renderTriangleToPPM(std::ofstream& ppmFs, const Grid& grid, const CRTriangle& triangle, std::vector<std::vector<CRTColor>>& pixelBuffer) {

    const CRTVector& normal = triangle.normalVector();
    const CRTVector& v0 = triangle.getVertices()[0];

    for (int i = 0; i < grid.imageHeight; i++) {
        for (int j = 0; j < grid.imageWidth; j++) {

            CRTVector cameraRay = CameraRay::generateCameraRay(grid, j, i, Z_DIR);
            float dotNormalCameraRay = dotProduct(normal, cameraRay);
            float dotNormalToVertex0 = dotProduct(normal, v0);

            if (areEqualFloats(dotNormalCameraRay, 0) || dotNormalToVertex0 >= 0.f) {
                continue;
            }

            float t = -dotNormalToVertex0 / dotNormalCameraRay;
            CRTVector intersectionPoint = t * cameraRay;

            if (isPointInsideTriangle(intersectionPoint, triangle)) {
                pixelBuffer[i][j].setGreen(255);
                pixelBuffer[i][j].setBlue(255);
            }
        }
    }
}

void constructTwoTriangles1(const Grid& grid, const CRTriangle& triangle1, const CRTriangle& triangle2) {

    std::ofstream ppmFs("output_image5.ppm", std::ios::out | std::ios::binary);
    ppmFs << "P3\n";
    ppmFs << grid.imageWidth << " " << grid.imageHeight << "\n";
    ppmFs << CRTColor::maxColorComponent << "\n";

    std::vector<std::vector<CRTColor>> pixelBuffer(grid.imageHeight, std::vector<CRTColor>(grid.imageWidth));

    renderTriangleToPPM(ppmFs, grid, triangle1, pixelBuffer);
    renderTriangleToPPM(ppmFs, grid, triangle2, pixelBuffer);

    for (int i = 0; i < grid.imageHeight; i++) {

        for (int j = 0; j < grid.imageWidth; j++) {

            ppmFs << pixelBuffer[i][j] << "\t";
        }

        ppmFs << "\n";
    }
}

void constructTwoTriangles2(const Grid& grid, const CRTriangle& triangle1, const CRTriangle& triangle2) {

    std::ofstream ppmFs("output_image5.ppm", std::ios::out | std::ios::binary);
    ppmFs << "P3\n";
    ppmFs << grid.imageWidth << " " << grid.imageHeight << "\n";
    ppmFs << CRTColor::maxColorComponent << "\n";

    for (int i = 0; i < grid.imageHeight; i++) {
        for (int j = 0; j < grid.imageWidth; j++) {

            CRTVector cameraRay = CameraRay::generateCameraRay(grid, j, i, Z_DIR);
            CRTColor pixelColor;

            
            const CRTVector& normal1 = triangle1.normalVector();
            const CRTVector& v01 = triangle1.getVertices()[0];

            float dotRay1 = dotProduct(normal1, cameraRay);
            float dotToV01 = dotProduct(normal1, v01);

            if (!areEqualFloats(dotRay1, 0) && dotToV01 < 0.f) {
                float t1 = -dotToV01 / dotRay1;
                CRTVector intersection1 = t1 * cameraRay;

                if (isPointInsideTriangle(intersection1, triangle1)) {
                    pixelColor.setGreen(255);
                    pixelColor.setBlue(255);
                }
            }
            
            const CRTVector& normal2 = triangle2.normalVector();
            const CRTVector& v02 = triangle2.getVertices()[0];

            float dotRay2 = dotProduct(normal2, cameraRay);
            float dotToV02 = dotProduct(normal2, v02);

            if (!areEqualFloats(dotRay2, 0) && dotToV02 < 0.f) {
                float t2 = -dotToV02 / dotRay2;
                CRTVector intersection2 = t2 * cameraRay;

                if (isPointInsideTriangle(intersection2, triangle2)) {
                    pixelColor.setGreen(255);
                    pixelColor.setBlue(255);
                }
            }

            ppmFs << pixelColor << "\t";
        }
        ppmFs << "\n";
    }
}

void construct3DShape(const Grid& grid, const std::vector<CRTriangle>& triangles) {

    std::ofstream ppmFs("output_image6.ppm", std::ios::out | std::ios::binary);
    ppmFs << "P3\n";
    ppmFs << grid.imageWidth << " " << grid.imageHeight << "\n";
    ppmFs << CRTColor::maxColorComponent << "\n";

    std::vector<CRTColor> triangleColors(triangles.size());

    for (int i = 0; i < triangleColors.size(); i++) {
        triangleColors[i] = CRTColor::generateRandomColor();
    }

    for (int i = 0; i < grid.imageHeight; i++) {

        for (int j = 0; j < grid.imageWidth; j++) {

            float minT = std::numeric_limits<float>::max();
            CRTVector cameraRay = CameraRay::generateCameraRay(grid, j, i, Z_DIR);
            CRTColor pixelColor;
            
            for (int k = 0; k < triangles.size(); k++) {

                const CRTVector& normal = triangles[k].normalVector();
                const CRTVector& v0 = triangles[k].getVertices()[0];

                float dotNormalCameraRay = dotProduct(normal, cameraRay);
                float dotNormalToVertex0 = dotProduct(normal, v0);

                if (areEqualFloats(dotNormalCameraRay, 0) || dotNormalToVertex0 >= 0.f) {
                    continue;
                }

                float distanceToPlane = -dotNormalToVertex0;
                float projection = std::abs(dotNormalCameraRay);

                float t = distanceToPlane / projection;
                CRTVector intersectionPoint = t * cameraRay;

                if (isPointInsideTriangle(intersectionPoint, triangles[k])) {

                    if (t < minT) {

                        minT = t;
                        pixelColor = triangleColors[k];
                    }
                }
            }

            ppmFs << pixelColor << "\t";
        }

        ppmFs << "\n";
    }
}

void task1(const Grid& grid) {

    CRTVector v0(-1.75f, -1.75f, -3.f);
    CRTVector v1(1.75f, -1.75f, -3.f);
    CRTVector v2(0.f, 1.75f, -3.f);

    CRTriangle triangle(v0, v1, v2);
    constructTriangle(grid, triangle);

}

void task2(const Grid& grid) {

    CRTVector v0(-2.f, -1.f, -3.f);
    CRTVector v1(2.f, -1.f, -3.f);
    CRTVector v2(0.f, -0.2f, -3.f);

    CRTriangle triangle(v0, v1, v2);
    constructTriangle(grid, triangle);
}

void task3(const Grid& grid) {

    CRTVector t1_v0(-2.0f, -1.0f, -3.0f);
    CRTVector t1_v1(2.0f, -1.0f, -3.0f);
    CRTVector t1_v2(0.0f, 2.0f, -3.0f);
    CRTriangle t1(t1_v0, t1_v1, t1_v2);
    t1.ensureCCWOrientation(CRTVector(0.f, 0.f, Z_DIR));

    CRTVector t2_v0(-2.0f, -1.0f, -3.0f);
    CRTVector t2_v1(2.0f, -1.0f, -3.0f);
    CRTVector t2_v2(4.5f, 2.0f, -3.0f);
    CRTriangle t2(t2_v0, t2_v1, t2_v2);
    t2.ensureCCWOrientation(CRTVector(0.f, 0.f, Z_DIR));

    constructTwoTriangles1(grid, t1, t2);
}

void task4(const Grid& grid) {

    std::vector<CRTriangle> triangles;

    CRTVector top(0.0f, 1.0f, -4.0f);
    CRTVector base1(-1.0f, -1.0f, -5.0f);
    CRTVector base2(1.0f, -1.0f, -5.0f);
    CRTVector base3(1.0f, -1.0f, -6.0f);
    CRTVector base4(-1.0f, -1.0f, -6.0f);

    triangles.emplace_back(CRTriangle(top, base1, base2));
    triangles.emplace_back(CRTriangle(top, base2, base3));
    triangles.emplace_back(CRTriangle(top, base3, base4));
    triangles.emplace_back(CRTriangle(top, base4, base1));
    triangles.emplace_back(CRTriangle(base1, base2, base3));
    triangles.emplace_back(CRTriangle(base1, base3, base4));

    construct3DShape(grid, triangles);
}

int main()
{
    Grid grid{ IMAGE_WIDTH, IMAGE_HEIGHT };
    task4(grid);
}
