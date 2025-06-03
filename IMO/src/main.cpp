#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <limits>
#include <random>
#include <chrono>

#include "point.h"
#include "utils.h"
#include "runtimes.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./tsp-study TSPlib95/{test-file}.tsp" << std::endl;
        return -1;
    }
    std::string filepath = argv[1];
    std::vector<Point> points = loadPointsFromFile(filepath);
    if (points.empty())
    {
        std::cerr << "No points loaded from the file!" << std::endl;
        return -1;
    }

    // Create the adjacency list
    std::vector<std::vector<double>> distanceMatrix = createdistanceMatrix(points);

    convexTestRuntime(points, distanceMatrix);

    return 0;
}