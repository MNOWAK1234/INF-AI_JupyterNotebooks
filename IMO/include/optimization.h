#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>

void enqueueImprovingVertexMoves(
    const std::vector<int> &pointsToUpdate,
    const Solution &solution,
    const std::vector<std::vector<double>> &distanceMatrix,
    const std::vector<int> &cycleSizes,
    std::priority_queue<Move> &pq,
    double epsilon);

void enqueueImprovingEdgeMoves(
    const std::vector<Cut> &cutsToUpdate,
    const Solution &solution,
    const std::vector<std::vector<double>> &distanceMatrix,
    const std::vector<int> &cycleSizes,
    std::priority_queue<Move> &pq,
    double epsilon);

Solution localSearchMemory(Solution solution, const std::vector<std::vector<double>> &distanceMatrix);

std::vector<std::vector<int>> computeNearestneighbours(const std::vector<std::vector<double>> &distanceMatrix, int n);

Solution localSearchCandidates(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, int neighbourhoodSize);

#endif