#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <vector>

Solution randomCycle(const std::vector<std::vector<double>> &distanceMatrix);

bool stepPointExchange(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy);

bool stepEdgeExchange(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy);

Solution localSearchVertex(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy);

Solution localSearchEdges(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, const std::string &strategy);

Solution randomWalk(Solution &solution, const std::vector<std::vector<double>> &distanceMatrix, int timeLimit);

#endif