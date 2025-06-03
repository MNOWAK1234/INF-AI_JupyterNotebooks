#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <limits>
#include <random>
#include <chrono>
#include <queue>
#include <algorithm>

#include "point.h"
#include "solution.h"
#include "move.h"
#include "localsearch.h"
#include "optimization.h"
#include "extendedlocalsearch.h"
#include "utils.h"
#include "greedyheuristics.h"
#include "evolutionaryheuristics.h"

int commonVerticesMetric(Solution &a, Solution &b)
{
    int count = 0;
    size_t size = a.pointPositions.size();
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = i + 1; j < size; ++j)
        {
            auto [cycleA, positionA] = a.getPointPosition(i);
            auto [cycleB, positionB] = b.getPointPosition(j);
            if (cycleA == cycleB)
            {
                count++;
            }
        }
    }
    return count;
}

int commonEdgesMetric(Solution &a, Solution &b)
{
    int count = 0;
    size_t pointsInCycle1 = a.cycleIndices[0].size();
    size_t pointsInCycle2 = a.cycleIndices[1].size();
    std::vector<size_t> cycleSizes = {pointsInCycle1, pointsInCycle2};
    for (int cycle = 0; cycle < 2; ++cycle)
    {
        for (size_t i = 0; i < a.cycleIndices[cycle].size(); i++)
        {
            // get edge
            int current = a.cycleIndices[cycle][i];
            int neighbor = a.cycleIndices[cycle][(i - 1 + cycleSizes[cycle]) % cycleSizes[cycle]];

            auto [cycleCurrent, positionCurrent] = b.getPointPosition(current);
            auto [cycleNeighbor, positionNeigbor] = b.getPointPosition(neighbor);

            if (cycleCurrent == cycleNeighbor)
            {
                int distance = abs(positionCurrent - positionNeigbor);
                if (distance == 1 || distance == static_cast<int>(cycleSizes[cycle] - 1))
                {
                    count++;
                }
            }
        }
    }
    return count;
}

void convexTest(Solution &goodSolution, std::vector<Solution> &solutions, const std::vector<std::vector<double>> &distanceMatrix, std::string metric = "vertices")
{
    // sort solutions
    sort(solutions.begin(), solutions.end());

    // get similarity between solutions and good solutions
    std::vector<double> similarities;
    double result;
    for (int i = 0; i < static_cast<int>(solutions.size()); ++i)
    {
        if (metric == "vertices")
            result = (double)commonVerticesMetric(goodSolution, solutions[i]);
        else
            result = (double)commonEdgesMetric(goodSolution, solutions[i]);

        similarities.push_back(result);
    }

    // draw plot
    std::string title = "Podobieństwo do bardzo dobrego rozwiązania";
    if (metric == "vertices")
        title += ", miara wierzchołków";
    else
        title += ", miara krawędzi";
    plotSimilarity(solutions, similarities, distanceMatrix, title);

    // get average similarity betweens solutions
    std::vector<double> averageimilarities;
    int similaritySum;
    double average;
    std::cout << "Calculating similarity" << std::endl;
    for (int i = 0; i < static_cast<int>(solutions.size()); ++i)
    {
        if (i % 100 == 0)
        {
            std::cout << "Progress: " << i << "/1000" << std::endl;
        }
        similaritySum = 0;
        for (int j = 0; j < static_cast<int>(solutions.size()); ++j)
        {
            if (i == j)
                continue;

            if (metric == "vertices")
                similaritySum += commonVerticesMetric(solutions[i], solutions[j]);
            else
                similaritySum += commonEdgesMetric(solutions[i], solutions[j]);
        }
        average = (double)similaritySum / 1000;
        averageimilarities.push_back(average);
    }

    // draw plot
    title = "Średnie podobieństwo";
    if (metric == "vertices")
        title += ", miara wierzchołków";
    else
        title += ", miara krawędzi";
    plotSimilarity(solutions, averageimilarities, distanceMatrix, title);
}