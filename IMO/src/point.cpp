#include <vector>
#include <cmath>

#include "point.h"

double euclideanDistance(Point a, Point b)
{
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

int findFurthestPointIndex(const std::vector<std::vector<double>> &distanceMatrix, int start)
{
    int n = distanceMatrix.size();
    int furthestPointIndex = -1;
    double maxDistance = -1;

    // Iterate over all points to find the one furthest from the start
    for (int i = 0; i < n; ++i)
    {
        if (i != start)
        {
            double distance = distanceMatrix[start][i];
            if (distance > maxDistance)
            {
                maxDistance = distance;
                furthestPointIndex = i;
            }
        }
    }

    return furthestPointIndex;
}