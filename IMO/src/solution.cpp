#include <vector>

#include "solution.h"

Solution::Solution() : cycle1Score(0.0), cycle2Score(0.0), score(0.0) {}

void Solution::calculateScore(const std::vector<std::vector<double>> &distanceMatrix)
{
    cycle1Score = calculateCycleDistance(cycleIndices[0], distanceMatrix);
    cycle2Score = calculateCycleDistance(cycleIndices[1], distanceMatrix);
    score = cycle1Score + cycle2Score;
}

void Solution::modifyScore(double change)
{
    score += change;
}

double Solution::getScore() const
{
    return score;
}

double Solution::getCycle1Score(const std::vector<std::vector<double>> &distanceMatrix)
{
    cycle1Score = calculateCycleDistance(cycleIndices[0], distanceMatrix);
    return cycle1Score;
}

double Solution::getCycle2Score(const std::vector<std::vector<double>> &distanceMatrix)
{
    cycle2Score = calculateCycleDistance(cycleIndices[1], distanceMatrix);
    return cycle2Score;
}

double Solution::calculateCycleDistance(const std::vector<int> &cycle, const std::vector<std::vector<double>> &distanceMatrix) const
{
    double totalDistance = 0.0;
    int cycleSize = cycle.size();
    for (int i = 0; i < cycleSize; ++i)
    {
        int current = cycle[i];
        int next = cycle[(i + 1) % cycleSize]; // Wrap around to form a cycle
        totalDistance += distanceMatrix[current][next];
    }
    return totalDistance;
}

void Solution::updatePointPositions()
{
    int totalPoints = cycleIndices[0].size() + cycleIndices[1].size();

    pointPositions.clear();
    pointPositions.resize(totalPoints, {-1, -1});

    for (int cycle = 0; cycle < 2; ++cycle)
    {
        for (int pos = 0; pos < static_cast<int>(cycleIndices[cycle].size()); ++pos)
        {
            int point = cycleIndices[cycle][pos];
            pointPositions[point] = {cycle, pos};
        }
    }
}

std::pair<int, int> Solution::getPointPosition(int pointIndex) const
{
    return pointPositions[pointIndex];
}