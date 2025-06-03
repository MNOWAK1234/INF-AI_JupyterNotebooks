#ifndef EXTENDEDLOCALSEARCH_H
#define EXTENDEDLOCALSEARCH_H

#include <vector>
#include <utility>

Solution multipleStartLocalSearch(const std::vector<std::vector<double>> &distanceMatrix);

void smallPermutation(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix);

std::pair<Solution, int> iteratedLocalSearch(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit);

void largePermutation(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix);

std::pair<Solution, int> largeNeighborhoodSearch(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit, bool localSearch);

#endif