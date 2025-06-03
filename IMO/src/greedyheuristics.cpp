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
#include "greedyheuristics.h"

Solution greedyNearestNeighbour(const std::vector<std::vector<double>> &distanceMatrix)
{
    Solution solution;
    int n = distanceMatrix.size();

    // Randomly select the starting points for the 1st cycle
    int start1 = std::rand() % n;
    int start2 = findFurthestPointIndex(distanceMatrix, start1);

    // Greedily build the first cycle starting from start1 and start2
    solution.cycleIndices[0].push_back(start1);
    solution.cycleIndices[1].push_back(start2);
    std::vector<bool> visited(n, false);
    visited[start1] = true;
    visited[start2] = true;
    int current1 = start1;
    int current2 = start2;
    // If there is an odd number of points then in 1 cycle there must be 1 vertex more
    // Example: 3 vertices = 2 + 1
    // And (3+1)/2 = 2
    // That's why +1
    int maxCycleSize = (n + 1) / 2;
    // 2 vertices are already visited so we check n-2 times
    for (int i = 2; i < n; ++i)
    {
        double minDistance = std::numeric_limits<double>::max();
        int nextPoint = -1;
        int addToCycle = -1;

        // Find the nearest unvisited neighbor using the distanceMatrix
        for (int j = 0; j < n; ++j)
        {
            if (!visited[j] && distanceMatrix[current1][j] < minDistance && static_cast<int>(solution.cycleIndices[0].size()) < maxCycleSize)
            {
                minDistance = distanceMatrix[current1][j];
                nextPoint = j;
                addToCycle = 1;
            }
            if (!visited[j] && distanceMatrix[current2][j] < minDistance && static_cast<int>(solution.cycleIndices[1].size()) < maxCycleSize)
            {
                minDistance = distanceMatrix[current2][j];
                nextPoint = j;
                addToCycle = 2;
            }
        }
        // Add best vertex to the appropriate cycle
        visited[nextPoint] = true;
        if (addToCycle == 1)
        {
            solution.cycleIndices[0].push_back(nextPoint);
            current1 = nextPoint;
        }
        else
        {
            solution.cycleIndices[1].push_back(nextPoint);
            current2 = nextPoint;
        }
    }
    solution.updatePointPositions();
    return solution;
}

Solution greedyCycle(const std::vector<std::vector<double>> &distanceMatrix)
{
    Solution solution;
    int n = distanceMatrix.size();

    // Randomly select the starting point for the 1st cycle
    int start1 = std::rand() % n;
    int start2 = findFurthestPointIndex(distanceMatrix, start1);

    // Initialize the cycles with the starting points
    solution.cycleIndices[0].push_back(start1);
    solution.cycleIndices[1].push_back(start2);
    std::vector<bool> visited(n, false);
    visited[start1] = true;
    visited[start2] = true;

    int maxCycleSize = (n + 1) / 2;

    // Greedily build the cycles, inserting the best vertex at the best position
    for (int i = 2; i < n; ++i)
    {
        double minIncrease = std::numeric_limits<double>::max();
        int nextPoint = -1;
        int addToCycle = -1;
        int insertPosition = -1;

        // Try to insert the new vertex in all possible positions in both cycles
        for (int j = 0; j < n; ++j)
        {
            if (!visited[j] && static_cast<int>(solution.cycleIndices[0].size()) < maxCycleSize)
            {
                // Try inserting in cycle 1
                for (size_t k = 0; k < solution.cycleIndices[0].size(); ++k)
                {
                    int prev = solution.cycleIndices[0][k];
                    int next = solution.cycleIndices[0][(k + 1) % static_cast<int>(solution.cycleIndices[0].size())];
                    double increase = distanceMatrix[prev][j] + distanceMatrix[j][next] - distanceMatrix[prev][next];

                    if (increase < minIncrease)
                    {
                        minIncrease = increase;
                        nextPoint = j;
                        addToCycle = 1;
                        insertPosition = k + 1;
                    }
                }
            }

            if (!visited[j] && static_cast<int>(solution.cycleIndices[1].size()) < maxCycleSize)
            {
                // Try inserting in cycle 2
                for (size_t k = 0; k < solution.cycleIndices[1].size(); ++k)
                {
                    int prev = solution.cycleIndices[1][k];
                    int next = solution.cycleIndices[1][(k + 1) % static_cast<int>(solution.cycleIndices[1].size())];
                    double increase = distanceMatrix[prev][j] + distanceMatrix[j][next] - distanceMatrix[prev][next];

                    if (increase < minIncrease)
                    {
                        minIncrease = increase;
                        nextPoint = j;
                        addToCycle = 2;
                        insertPosition = k + 1;
                    }
                }
            }
        }

        // Insert the best vertex into the chosen cycle at the determined position
        visited[nextPoint] = true;
        if (addToCycle == 1)
        {
            solution.cycleIndices[0].insert(solution.cycleIndices[0].begin() + insertPosition, nextPoint);
        }
        else
        {
            solution.cycleIndices[1].insert(solution.cycleIndices[1].begin() + insertPosition, nextPoint);
        }
    }
    solution.updatePointPositions();
    return solution;
}

Solution regretCycleWeighted(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, double weightCost = 0.0, double weightRegret = 1.0)
{
    int n = distanceMatrix.size();
    std::vector<bool> visited(n, false);

    // If we start constructing the solution from scratch
    if (solution.cycleIndices[0].size() == 0 && solution.cycleIndices[1].size() == 0)
    {
        // Randomly select the starting points for the two cycles
        int start1 = std::rand() % n;
        int start2 = findFurthestPointIndex(distanceMatrix, start1);

        // Initialize cycles with starting points
        solution.cycleIndices[0].push_back(start1);
        solution.cycleIndices[1].push_back(start2);
        visited[start1] = true;
        visited[start2] = true;
    }
    else
    {
        // Mark already added points as visited
        for (size_t i = 0; i < solution.cycleIndices[0].size(); i++)
        {
            visited[solution.cycleIndices[0][i]] = true;
        }
        for (size_t i = 0; i < solution.cycleIndices[1].size(); i++)
        {
            visited[solution.cycleIndices[1][i]] = true;
        }
    }

    int alreadyAddedCount = solution.cycleIndices[0].size() + solution.cycleIndices[1].size();
    int maxCycleSize = (n + 1) / 2;

    // Greedily build the cycles using regret heuristic
    for (int i = alreadyAddedCount; i < n; ++i)
    {
        int nextPoint = -1;
        int bestCycle = -1;
        int bestInsertPos = -1;
        double minScore = std::numeric_limits<double>::max();

        // Try inserting each unvisited point
        for (int j = 0; j < n; ++j)
        {
            if (visited[j])
                continue;

            double bestCost1 = std::numeric_limits<double>::max();
            double bestCost2 = std::numeric_limits<double>::max();
            int bestPos1 = -1, bestPos2 = -1;

            // Find best insertion cost for cycle 1
            if (static_cast<int>(solution.cycleIndices[0].size()) < maxCycleSize)
            {
                for (size_t idx = 0; idx < solution.cycleIndices[0].size(); ++idx)
                {
                    int prev = solution.cycleIndices[0][idx];
                    int next = solution.cycleIndices[0][(idx + 1) % static_cast<int>(solution.cycleIndices[0].size())];
                    double cost = distanceMatrix[prev][j] + distanceMatrix[j][next] - distanceMatrix[prev][next];

                    if (cost < bestCost1)
                    {
                        bestCost1 = cost;
                        bestPos1 = idx + 1;
                    }
                }
            }

            // Find best insertion cost for cycle 2
            if (static_cast<int>(solution.cycleIndices[1].size()) < maxCycleSize)
            {
                for (size_t idx = 0; idx < solution.cycleIndices[1].size(); ++idx)
                {
                    int prev = solution.cycleIndices[1][idx];
                    int next = solution.cycleIndices[1][(idx + 1) % solution.cycleIndices[1].size()];
                    double cost = distanceMatrix[prev][j] + distanceMatrix[j][next] - distanceMatrix[prev][next];

                    if (cost < bestCost2)
                    {
                        bestCost2 = cost;
                        bestPos2 = idx + 1;
                    }
                }
            }

            // Compute regret (difference between best insertions in cycle1 and cycle2)
            double score = std::min(bestCost1, bestCost2) * weightCost - std::abs(bestCost1 - bestCost2) * weightRegret;

            // Choose the city with the highest regret
            if (score < minScore)
            {
                minScore = score;
                nextPoint = j;

                if (bestCost1 < bestCost2)
                {
                    bestCycle = 1;
                    bestInsertPos = bestPos1;
                }
                else
                {
                    bestCycle = 2;
                    bestInsertPos = bestPos2;
                }
            }
        }

        // Insert the selected city at the best position in the chosen cycle
        visited[nextPoint] = true;
        if (bestCycle == 1)
        {
            solution.cycleIndices[0].insert(solution.cycleIndices[0].begin() + bestInsertPos, nextPoint);
        }
        else
        {
            solution.cycleIndices[1].insert(solution.cycleIndices[1].begin() + bestInsertPos, nextPoint);
        }
    }
    solution.updatePointPositions();
    return solution;
}