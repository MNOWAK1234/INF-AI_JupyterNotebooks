#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <cmath>

struct Solution
{
    std::vector<int> cycleIndices[2];
    double cycle1Score;
    double cycle2Score;
    double score;
    std::vector<std::pair<int, int>> pointPositions;

    Solution();

    void calculateScore(const std::vector<std::vector<double>> &distanceMatrix);

    void modifyScore(double change);

    double getScore() const;

    double getCycle1Score(const std::vector<std::vector<double>> &distanceMatrix);

    double getCycle2Score(const std::vector<std::vector<double>> &distanceMatrix);

    double calculateCycleDistance(const std::vector<int> &cycle, const std::vector<std::vector<double>> &distanceMatrix) const;

    void updatePointPositions();

    std::pair<int, int> getPointPosition(int pointIndex) const;

    bool operator<(const Solution &other) const
    {
        return score < other.score; // ascending
    }

    static constexpr double EPSILON = 1e-9;
    bool operator==(const Solution &other) const
    {
        return abs(score - other.score) < EPSILON;
    }
    bool operator!=(const Solution &other) const
    {
        return !(*this == other);
    }
};

#endif