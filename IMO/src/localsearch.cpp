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

Solution randomCycle(const std::vector<std::vector<double>> &distanceMatrix)
{
    Solution solution;
    int n = distanceMatrix.size();

    // int seed = 41; // dowolna liczba całkowita jako seed
    // std::mt19937 rng(seed);

    // Generate a shuffled list of indices
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));
    // std::shuffle(indices.begin(), indices.end(), rng);

    // Create a vector with half 0s and half 1s
    std::vector<int> cycleAssignment(n, 0);
    std::fill(cycleAssignment.begin() + n / 2, cycleAssignment.end(), 1);
    std::shuffle(cycleAssignment.begin(), cycleAssignment.end(), std::mt19937(std::random_device()()));
    // std::shuffle(cycleAssignment.begin(), cycleAssignment.end(), rng);

    // Assign points to cycles based on the cycleAssignment vector
    for (int i = 0; i < n; ++i)
    {
        if (cycleAssignment[i] == 0)
            solution.cycleIndices[0].push_back(indices[i]);
        else
            solution.cycleIndices[1].push_back(indices[i]);
    }
    solution.updatePointPositions();
    return solution;
}

bool stepPointExchange(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy)
{
    const double epsilon = 1e-6; // Avoid numerical errors
    size_t numberOfPoints = distanceMatrix.size();
    size_t pointsInCycle1 = solution.cycleIndices[0].size(); // Size of first cycle
    size_t pointsInCycle2 = solution.cycleIndices[1].size(); // Size of second cycle

    std::vector<size_t> cycleSizes = {pointsInCycle1, pointsInCycle2};

    double minDelta = 0.0;
    int besti = -1;
    int bestj = -1;
    bool foundSwap = false;

    int point1;
    int point2;
    int cycleOfPoint1;
    int cycleOfPoint2;

    // Create and shuffle the index vector
    std::vector<size_t> indices(numberOfPoints);
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, ..., numberOfPoints - 1

    // Random number generator for shuffling
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(indices.begin(), indices.end(), rng);

    // Iterate using the shuffled indices
    for (size_t a = 0; a < numberOfPoints; ++a)
    {
        size_t i = indices[a];
        for (size_t b = a + 1; b < numberOfPoints; ++b)
        {
            size_t j = indices[b];
            // Calculate to which cycle these point belong to
            // Example 13/50 = 0 (integer division) so it is the 1st cycle
            // Example 63/50 = 1 (integer division) so it is the 2nd cycle
            cycleOfPoint1 = i / pointsInCycle1;
            cycleOfPoint2 = j / pointsInCycle1;
            // Calculate how much can we gain by swapping these 2 points
            double delta = 0;
            // Get all the neighbours of point 1
            int next1;
            int prev1;
            point1 = solution.cycleIndices[cycleOfPoint1][i % pointsInCycle1];
            next1 = solution.cycleIndices[cycleOfPoint1][(i + 1) % cycleSizes[cycleOfPoint1]];
            prev1 = solution.cycleIndices[cycleOfPoint1][(i - 1 + cycleSizes[cycleOfPoint1]) % cycleSizes[cycleOfPoint1]];
            // Get all the neighbours of point 2
            int next2;
            int prev2;
            point2 = solution.cycleIndices[cycleOfPoint2][j % pointsInCycle1];
            next2 = solution.cycleIndices[cycleOfPoint2][(j + 1) % cycleSizes[cycleOfPoint2]];
            prev2 = solution.cycleIndices[cycleOfPoint2][(j - 1 + cycleSizes[cycleOfPoint2]) % cycleSizes[cycleOfPoint2]];
            // Calculate delta
            delta -= distanceMatrix[point1][next1];
            delta -= distanceMatrix[point1][prev1];
            delta -= distanceMatrix[point2][next2];
            delta -= distanceMatrix[point2][prev2];
            // Handle edge case when we have A----->point1----->point2----->B
            // Then distance[point1][prev2] is distance[point1][point1] and it should be distance[point1][point2]
            if (point2 == next1)
            {
                prev2 = point2;
                next1 = point1;
            }
            // Handle edge case when we have A----->point2----->point1----->B
            // Then distance[point1][next2] is distance[point1][point1] and it should be distance[point1][point2]
            if (point2 == prev1)
            {
                prev1 = point1;
                next2 = point2;
            }
            delta += distanceMatrix[point1][next2];
            delta += distanceMatrix[point1][prev2];
            delta += distanceMatrix[point2][next1];
            delta += distanceMatrix[point2][prev1];

            // Check if this is the best swap so far
            if (delta < minDelta - epsilon)
            {
                besti = i;
                bestj = j;
                minDelta = delta;
                foundSwap = true;
            }
            // But greedy LS means take 1st found
            if (strategy == "greedy" && delta < -epsilon)
            {
                // A swap was found
                solution.cycleIndices[cycleOfPoint1][i % pointsInCycle1] = point2;
                solution.cycleIndices[cycleOfPoint2][j % pointsInCycle1] = point1;
                return true;
            }
        }
    }
    if (!foundSwap)
    {
        return false;
    }
    else
    {
        // A swap was found
        cycleOfPoint1 = besti / pointsInCycle1;
        cycleOfPoint2 = bestj / pointsInCycle1;
        point1 = solution.cycleIndices[cycleOfPoint1][besti % pointsInCycle1];
        point2 = solution.cycleIndices[cycleOfPoint2][bestj % pointsInCycle1];
        solution.cycleIndices[cycleOfPoint1][besti % pointsInCycle1] = point2;
        solution.cycleIndices[cycleOfPoint2][bestj % pointsInCycle1] = point1;
        return true;
    }

    return foundSwap;
}

bool stepEdgeExchange(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy)
{
    const double epsilon = 1e-6; // Avoid numerical error
    size_t pointsInCycle1 = solution.cycleIndices[0].size();
    size_t pointsInCycle2 = solution.cycleIndices[1].size();
    std::vector<size_t> cycleSizes = {pointsInCycle1, pointsInCycle2};

    double minDelta = 0.0;
    int bestPos1 = -1;
    int bestPos2 = -1;
    int bestCycle = -1;
    bool foundSwap = false;
    bool swappingVertices = true;

    int point1, point2;
    int cycleOfPoint1 = 0;
    int cycleOfPoint2 = 1;

    Move bestMove;
    // Add vertex swap between 2 cycles
    // For some reason it should always be a legal move
    std::random_device rd;
    std::default_random_engine rng(rd());

    // Shuffle indices for cycle 1
    std::vector<size_t> indices1(pointsInCycle1);
    std::iota(indices1.begin(), indices1.end(), 0);
    std::shuffle(indices1.begin(), indices1.end(), rng);

    // Shuffle indices for cycle 2
    std::vector<size_t> indices2(pointsInCycle2);
    std::iota(indices2.begin(), indices2.end(), 0);
    std::shuffle(indices2.begin(), indices2.end(), rng);

    // Loop over the shuffled index pairs
    for (size_t i : indices1)
    {
        for (size_t j : indices2)
        {
            double delta = 0;
            // Get all the neighbours of point 1
            int next1;
            int prev1;
            point1 = solution.cycleIndices[cycleOfPoint1][i % pointsInCycle1];
            next1 = solution.cycleIndices[cycleOfPoint1][(i + 1) % cycleSizes[cycleOfPoint1]];
            prev1 = solution.cycleIndices[cycleOfPoint1][(i - 1 + cycleSizes[cycleOfPoint1]) % cycleSizes[cycleOfPoint1]];
            // Get all the neighbours of point 2
            int next2;
            int prev2;
            point2 = solution.cycleIndices[cycleOfPoint2][j % pointsInCycle1];
            next2 = solution.cycleIndices[cycleOfPoint2][(j + 1) % cycleSizes[cycleOfPoint2]];
            prev2 = solution.cycleIndices[cycleOfPoint2][(j - 1 + cycleSizes[cycleOfPoint2]) % cycleSizes[cycleOfPoint2]];
            // Calculate delta
            delta -= distanceMatrix[point1][next1];
            delta -= distanceMatrix[point1][prev1];
            delta -= distanceMatrix[point2][next2];
            delta -= distanceMatrix[point2][prev2];
            delta += distanceMatrix[point1][next2];
            delta += distanceMatrix[point1][prev2];
            delta += distanceMatrix[point2][next1];
            delta += distanceMatrix[point2][prev1];
            // Only add moves that can improve the solution
            if (strategy == "greedy" && delta < -epsilon)
            {
                int pos1 = i;
                int pos2 = j;
                // Ensure the points are in separate cycles
                if (cycleOfPoint1 != cycleOfPoint2)
                {
                    std::swap(solution.cycleIndices[cycleOfPoint1][pos1], solution.cycleIndices[cycleOfPoint2][pos2]);
                    std::swap(solution.pointPositions[point1], solution.pointPositions[point2]);
                }
                return true;
            }
            if (delta < minDelta - epsilon)
            {
                bestPos1 = i;
                bestPos2 = j;
                minDelta = delta;
                foundSwap = true;
            }
        }
    }
    for (size_t c = 0; c < 2; ++c)
    {
        // Create and shuffle index vector
        std::vector<size_t> indices(cycleSizes[c]);
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), rng);

        // Loop over shuffled pairs
        for (size_t ii = 0; ii < indices.size(); ++ii)
        {
            size_t i = indices[ii];
            for (size_t jj = 0; jj < indices.size(); ++jj)
            {
                size_t j = indices[jj];
                // Skip reversing the entire cycle
                if (i == j || (j + 1) % cycleSizes[c] == i || (j + 2) % cycleSizes[c] == i)
                {
                    continue;
                }
                double delta = 0;
                int prev1, next2;
                point1 = solution.cycleIndices[c][i];
                prev1 = solution.cycleIndices[c][(i - 1 + cycleSizes[c]) % cycleSizes[c]];
                point2 = solution.cycleIndices[c][j];
                next2 = solution.cycleIndices[c][(j + 1) % cycleSizes[c]];

                delta -= distanceMatrix[point1][prev1];
                delta -= distanceMatrix[point2][next2];
                delta += distanceMatrix[point1][next2];
                delta += distanceMatrix[point2][prev1];

                if (delta < minDelta - epsilon)
                {
                    bestMove = EdgeMove({prev1, point1}, {point2, next2}, delta);
                    bestCycle = c;
                    bestPos1 = i;
                    bestPos2 = j;
                    minDelta = delta;
                    foundSwap = true;
                    swappingVertices = false; // Change strategy
                }

                if (strategy == "greedy" && delta < -epsilon)
                {
                    bestPos1 = i;
                    bestPos2 = j;
                    if (bestPos1 < bestPos2)
                    {
                        // Normal case: reverse directly
                        std::reverse(solution.cycleIndices[bestCycle].begin() + bestPos1, solution.cycleIndices[bestCycle].begin() + bestPos2 + 1);
                        solution.updatePointPositions();
                    }
                    else
                    {
                        // Wrap-around case
                        std::reverse(solution.cycleIndices[bestCycle].begin() + bestPos2 + 1, solution.cycleIndices[bestCycle].begin() + bestPos1);
                        solution.updatePointPositions();
                    }
                    return true;
                }
            }
        }
    }

    if (!foundSwap)
    {
        return false;
    }
    else
    {
        // Check which type of a move to do
        if (swappingVertices)
        {
            //  Swap the points between the two cycles
            int pointToSwap1 = solution.cycleIndices[0][bestPos1];
            int pointToSwap2 = solution.cycleIndices[1][bestPos2];
            // std::cout << "Move vertices " << pointToSwap1 << " " << pointToSwap2 << " score " << minDelta << std::endl;
            std::swap(solution.cycleIndices[0][bestPos1], solution.cycleIndices[1][bestPos2]);
            std::swap(solution.pointPositions[pointToSwap1], solution.pointPositions[pointToSwap2]);
        }
        else
        {
            // Get current positions of the points in the cycles
            auto [cycle1, pos1] = solution.getPointPosition(bestMove.cut1.next);
            auto [cycle2, pos2] = solution.getPointPosition(bestMove.cut2.prev);
            int c = cycle1; // Both cycles are the same
            // Ensure position 1 is before position 2
            if (pos1 > pos2)
            {
                std::swap(pos1, pos2);
                std::swap(bestMove.cut1, bestMove.cut2);
            }
            std::reverse(solution.cycleIndices[c].begin() + pos1, solution.cycleIndices[c].begin() + pos2 + 1);
            solution.updatePointPositions();
            // std::cout << ": (" << bestMove.cut1.prev << " " << bestMove.cut1.next << ") and (" << bestMove.cut2.prev << " " << bestMove.cut2.next << ")" << " score " << bestMove.delta << std::endl;
        }
        return true;
    }
    return foundSwap;
}

Solution localSearchVertex(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy)
{
    bool improvement = true;
    while (improvement)
    {
        improvement = stepPointExchange(solution, distanceMatrix, strategy);
    }
    solution.updatePointPositions();
    return solution;
}

Solution localSearchEdges(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy)
{
    bool improvement = true;
    while (improvement)
    {
        improvement = stepEdgeExchange(solution, distanceMatrix, strategy);
    }
    solution.updatePointPositions();
    return solution;
}

Solution randomWalk(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix, int timeLimit = 1000)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    Solution bestSolution = solution;
    solution.calculateScore(distanceMatrix);
    double bestScore = solution.score;

    Solution currentSolution = solution;

    int pointsInCycle1 = static_cast<int>(solution.cycleIndices[0].size());
    int pointsInCycle2 = static_cast<int>(solution.cycleIndices[1].size());
    std::vector<int> cycleSizes = {pointsInCycle1, pointsInCycle2};

    auto t1 = std::chrono::high_resolution_clock::now();
    long runtime = 0;
    while (runtime < timeLimit)
    {
        Solution tempSolution = currentSolution;
        std::uniform_int_distribution<int> choiceDist(0, 1);
        bool swapVertices = choiceDist(gen); // Randomly choose to swap vertices or edges

        if (swapVertices)
        {
            // Swap two random vertices
            std::uniform_int_distribution<int> pointDist(0, pointsInCycle1 + pointsInCycle2 - 1);
            int index1 = pointDist(gen);
            int index2 = pointDist(gen);

            while (index1 == index2) // Ensure different indices
                index2 = pointDist(gen);

            int cycle1 = (index1 < pointsInCycle1) ? 0 : 1;
            int cycle2 = (index2 < pointsInCycle1) ? 0 : 1;
            index1 %= cycleSizes[cycle1];
            index2 %= cycleSizes[cycle2];

            std::swap(tempSolution.cycleIndices[cycle1][index1], tempSolution.cycleIndices[cycle2][index2]);
        }
        else
        {
            // Swap edges
            std::uniform_int_distribution<int> cycleDist(0, 1);
            int c = cycleDist(gen);
            std::uniform_int_distribution<int> indexDist(0, cycleSizes[c] - 1);

            int i = indexDist(gen);
            int j = indexDist(gen);
            while (j == 0 || j == cycleSizes[c] - 1) // Ensure valid range
                j = indexDist(gen);

            int endIdx = (i + j) % cycleSizes[c];

            if (endIdx > i)
            {
                // Normal case: reverse directly
                std::reverse(tempSolution.cycleIndices[c].begin() + i, tempSolution.cycleIndices[c].begin() + endIdx + 1);
            }
            else
            {
                // Wrap-around case
                std::vector<int> temp;

                // Move front elements to the back
                temp.insert(temp.end(), tempSolution.cycleIndices[c].begin() + endIdx + 1, tempSolution.cycleIndices[c].end());
                temp.insert(temp.end(), tempSolution.cycleIndices[c].begin(), tempSolution.cycleIndices[c].begin() + endIdx + 1);

                // Reverse the required section
                std::reverse(temp.begin(), temp.begin() + (cycleSizes[c] - i));

                // Restore back to the original vector
                for (size_t k = 0; k < temp.size(); ++k)
                {
                    tempSolution.cycleIndices[c][(i + k) % cycleSizes[c]] = temp[k];
                }
            }
        }

        tempSolution.calculateScore(distanceMatrix);
        double newScore = tempSolution.score;
        if (newScore < bestScore)
        {
            bestSolution = tempSolution;
            bestScore = newScore;
        }

        currentSolution = tempSolution; // Update current solution for the next iteration
        auto t2 = std::chrono::high_resolution_clock::now();
        runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    }
    bestSolution.updatePointPositions();
    return bestSolution;
}