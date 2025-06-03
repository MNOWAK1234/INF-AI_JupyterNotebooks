#ifndef POINT_H
#define POINT_H

#include <vector>

struct Point
{
    double x, y;
};

double euclideanDistance(Point a, Point b);

int findFurthestPointIndex(const std::vector<std::vector<double>> &distanceMatrix, int start);

#endif