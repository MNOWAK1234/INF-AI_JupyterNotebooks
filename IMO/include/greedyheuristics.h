#ifndef GREEDYHEURISTICS_H
#define GREEDYHEURISTICS_H

#include <vector>

Solution greedyNearestNeighbour(const std::vector<std::vector<double>> &distanceMatrix);

Solution greedyCycle(const std::vector<std::vector<double>> &distanceMatrix);

Solution regretCycleWeighted(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, double weightCost, double weightRegret);

#endif