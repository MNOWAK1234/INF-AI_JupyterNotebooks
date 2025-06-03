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
#include <utility>

#include "point.h"
#include "solution.h"
#include "move.h"
#include "localsearch.h"
#include "optimization.h"
#include "extendedlocalsearch.h"
#include "utils.h"
#include "greedyheuristics.h"

Solution multipleStartLocalSearch(const std::vector<std::vector<double>> &distanceMatrix)
{
    Solution bestSolution;
    double bestScore = std::numeric_limits<double>::max();

    for (int i = 0; i < 200; i++)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        Solution initialRandom = randomCycle(distanceMatrix);
        Solution newSolution = localSearchMemory(initialRandom, distanceMatrix);

        newSolution.calculateScore(distanceMatrix);
        double newScore = newSolution.score;
        if (newScore < bestScore)
        {
            bestSolution = newSolution;
            bestScore = newScore;
        }
    }
    return bestSolution;
}

void smallPermutation(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix)
{
    int totalPoints = solution.cycleIndices[0].size() + solution.cycleIndices[1].size();
    int numNeighbors = std::max(1, totalPoints * 3 / 200); // 3% of points, at least 1

    // Find random point from random cycle
    int cycle1 = std::rand() % 2;
    int idx1 = std::rand() % solution.cycleIndices[cycle1].size();
    int point1 = solution.cycleIndices[cycle1][idx1];

    // Find furthest point in other cycle
    int cycle2 = 1 - cycle1;
    int idx2 = -1;
    double maxDist = -1.0;
    for (size_t i = 0; i < solution.cycleIndices[cycle2].size(); i++)
    {
        int candidate = solution.cycleIndices[cycle2][i];
        double dist = distanceMatrix[point1][candidate];
        if (dist > maxDist)
        {
            maxDist = dist;
            idx2 = i;
        }
    }
    int point2 = solution.cycleIndices[cycle2][idx2];
    // Find points in cycle1 furthest from point2
    // and points in cycle2 furthest from point1
    int leftBoundaryPoint1 = idx1;
    int rightBoundaryPoint1 = idx1;
    int size1 = solution.cycleIndices[cycle1].size();
    int leftBoundaryPoint2 = idx2;
    int rightBoundaryPoint2 = idx2;
    int size2 = solution.cycleIndices[cycle2].size();
    for (int i = 0; i < numNeighbors - 1; i++)
    {
        int leftNeighbourSegmentPoint1 = solution.cycleIndices[cycle1][(leftBoundaryPoint1 - 1 + size1) % size1];
        int rightNeighbourSegmentPoint1 = solution.cycleIndices[cycle1][(rightBoundaryPoint1 + 1) % size1];
        if (distanceMatrix[point2][leftNeighbourSegmentPoint1] > distanceMatrix[point2][rightNeighbourSegmentPoint1])
        {
            leftBoundaryPoint1 -= 1;
            leftBoundaryPoint1 += size1;
            leftBoundaryPoint1 %= size1;
        }
        else
        {
            rightBoundaryPoint1 += 1;
            rightBoundaryPoint1 %= size1;
        }
        int leftNeighbourSegmentPoint2 = solution.cycleIndices[cycle2][(leftBoundaryPoint2 - 1 + size2) % size2];
        int rightNeighbourSegmentPoint2 = solution.cycleIndices[cycle2][(rightBoundaryPoint2 + 1) % size2];
        if (distanceMatrix[point1][leftNeighbourSegmentPoint2] > distanceMatrix[point1][rightNeighbourSegmentPoint2])
        {
            leftBoundaryPoint2 -= 1;
            leftBoundaryPoint2 += size2;
            leftBoundaryPoint2 %= size2;
        }
        else
        {
            rightBoundaryPoint2 += 1;
            rightBoundaryPoint2 %= size2;
        }
    }
    // Swap these 2 segments
    int segmentSize = numNeighbors;

    for (int i = 0; i < segmentSize; i++)
    {
        int idxInCycle1 = (leftBoundaryPoint1 + i) % size1;
        int idxInCycle2 = (leftBoundaryPoint2 + i) % size2;
        int first = solution.cycleIndices[cycle1][idxInCycle1];
        int second = solution.cycleIndices[cycle2][idxInCycle2];
        std::swap(solution.cycleIndices[cycle1][idxInCycle1], solution.cycleIndices[cycle2][idxInCycle2]);
        std::swap(solution.pointPositions[first], solution.pointPositions[second]);
    }
}

std::pair<Solution, int> iteratedLocalSearch(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    Solution x = randomCycle(distanceMatrix);
    x = localSearchMemory(x, distanceMatrix);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    long runtime = 0;
    int iterations = 0;

    while (runtime < timeLimit)
    {
        auto t2 = std::chrono::high_resolution_clock::now();
        runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        Solution y = x;
        smallPermutation(y, distanceMatrix);
        y = localSearchMemory(y, distanceMatrix);

        x.calculateScore(distanceMatrix);
        y.calculateScore(distanceMatrix);

        if (y.score < x.score)
            x = y;
        iterations++;
    }
    return std::make_pair(x, iterations);
}

void largePermutation(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix)
{
    int totalPoints = solution.cycleIndices[0].size() + solution.cycleIndices[1].size();
    int numPoints = std::max(1, totalPoints * 30 / 200); // 30% of points, at least 1

    // Find random point from random cycle
    int cycle1 = std::rand() % 2;
    int idx1 = std::rand() % solution.cycleIndices[cycle1].size();
    int point1 = solution.cycleIndices[cycle1][idx1];

    // Find furthest point in other cycle
    int cycle2 = 1 - cycle1;
    int idx2 = -1;
    double maxDist = -1.0;
    for (size_t i = 0; i < solution.cycleIndices[cycle2].size(); i++)
    {
        int candidate = solution.cycleIndices[cycle2][i];
        double dist = distanceMatrix[point1][candidate];
        if (dist > maxDist)
        {
            maxDist = dist;
            idx2 = i;
        }
    }
    int point2 = solution.cycleIndices[cycle2][idx2];

    std::vector<bool> toRemoveCycle1(solution.cycleIndices[cycle1].size(), false);
    std::vector<bool> toRemoveCycle2(solution.cycleIndices[cycle2].size(), false);

    // Mark points in cycle1 furthest from point2
    std::vector<std::pair<double, int>> distIdx1;
    for (size_t i = 0; i < solution.cycleIndices[cycle1].size(); i++)
    {
        int p = solution.cycleIndices[cycle1][i];
        double d = distanceMatrix[point2][p];
        distIdx1.push_back({d, i});
    }
    std::sort(distIdx1.rbegin(), distIdx1.rend()); // Sort descending by distance
    for (size_t i = 0; i < std::min<size_t>(numPoints, distIdx1.size()); i++)
    {
        toRemoveCycle1[distIdx1[i].second] = true;
    }

    // Mark points in cycle2 furthest from point1
    std::vector<std::pair<double, int>> distIdx2;
    for (size_t i = 0; i < solution.cycleIndices[cycle2].size(); i++)
    {
        int p = solution.cycleIndices[cycle2][i];
        double d = distanceMatrix[point1][p];
        distIdx2.push_back({d, i});
    }
    std::sort(distIdx2.rbegin(), distIdx2.rend()); // Sort descending by distance
    for (size_t i = 0; i < std::min<size_t>(numPoints, distIdx2.size()); i++)
    {
        toRemoveCycle2[distIdx2[i].second] = true;
    }

    // Create new cycles without the removed points
    std::vector<int> newCycle1;
    for (size_t i = 0; i < solution.cycleIndices[cycle1].size(); i++)
    {
        if (!toRemoveCycle1[i])
            newCycle1.push_back(solution.cycleIndices[cycle1][i]);
    }

    std::vector<int> newCycle2;
    for (size_t i = 0; i < solution.cycleIndices[cycle2].size(); i++)
    {
        if (!toRemoveCycle2[i])
            newCycle2.push_back(solution.cycleIndices[cycle2][i]);
    }

    solution.cycleIndices[cycle1] = newCycle1;
    solution.cycleIndices[cycle2] = newCycle2;

    // Rebuild solution using wighted regret heuristic
    solution = regretCycleWeighted(solution, distanceMatrix, 1.0, 1.0);
}

std::pair<Solution, int> largeNeighborhoodSearch(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit, bool localSearch = true)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    Solution x = randomCycle(distanceMatrix);
    x = localSearchMemory(x, distanceMatrix);

    long runtime = 0;
    int iterations = 0;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (runtime < timeLimit)
    {
        auto t2 = std::chrono::high_resolution_clock::now();
        runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        Solution y = x;
        largePermutation(y, distanceMatrix);

        if (localSearch)
            y = localSearchMemory(y, distanceMatrix);

        x.calculateScore(distanceMatrix);
        y.calculateScore(distanceMatrix);

        if (y.score < x.score)
            x = y;
        iterations++;
    }

    return std::make_pair(x, iterations);
}