#include "Circle.h"
#include <iostream>

constexpr int MAX_RADIUS = 1080;

static int generateCircleRadius()
{
    // the number is created using <random> library  
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, MAX_RADIUS);
    return dist(gen);
}

Circle::Circle(int x, int y)
{
    radius = generateCircleRadius();
    circleCenter.x = x;
    circleCenter.y = y;
    color = Color::generateRandomColor();
}

bool Circle::isPointInCircle(const Point& point) const
{
    int distance = circleCenter.getDistanceToPoint(point);
    return distance <= radius;
}

const Color& Circle::getColor() const
{
    return color;
}
