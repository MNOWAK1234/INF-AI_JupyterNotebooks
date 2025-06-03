#ifndef CONVEXTEST_H
#define CONVEXTEST_H

#include <vector>
#include <utility>

int commonVerticesMetric(Solution &a, Solution &b);

int commonEdgesMetric(Solution &a, Solution &b);

void convexTest(Solution &goodSolution, std::vector<Solution> &solutions, const std::vector<std::vector<double>> &distanceMatrix, std::string metric);

#endif