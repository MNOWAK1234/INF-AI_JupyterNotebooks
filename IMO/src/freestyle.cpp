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
#include "optimization.h"
#include "extendedlocalsearch.h"
#include "utils.h"
#include "greedyheuristics.h"
#include "evolutionaryheuristics.h"
#include "freestyle.h"

#define POPULATION_SIZE 20

std::pair<Solution, int> freestyle(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<Solution> population;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 3); // 4 choices: 0-3
    std::uniform_int_distribution<int> localSearchWeightDist(1, 100);
    while (population.size() < POPULATION_SIZE)
    {
        Solution x;
        int method = dist(rng);
        switch (method)
        {
        case 0:
            x = randomCycle(distanceMatrix);
            break;
        case 1:
            x = greedyNearestNeighbour(distanceMatrix);
            break;
        case 2:
            x = greedyCycle(distanceMatrix);
            break;
        case 3:
        {
            Solution empty;
            x = regretCycleWeighted(empty, distanceMatrix, 1.0, 1.0);
            break;
        }
        }
        int roll = localSearchWeightDist(rng);
        if (roll <= 90)
        {
            x = localSearchMemory(x, distanceMatrix);
        }
        else
        {
            std::uniform_int_distribution<int> nhSizeDist(1, distanceMatrix.size() / 2 - 2);
            int neighbourhoodSize = nhSizeDist(rng);
            x = localSearchCandidates(x, distanceMatrix, neighbourhoodSize);
        }
        x.calculateScore(distanceMatrix);
        insertIntoPopulation(population, x, POPULATION_SIZE);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    long runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    int iterations = 0;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (runtime < timeLimit)
    {
        t2 = std::chrono::high_resolution_clock::now();
        runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        // Select two distinct parents uniformly at random
        int idx1 = std::rand() % POPULATION_SIZE;
        int idx2;
        do
        {
            idx2 = std::rand() % POPULATION_SIZE;
        } while (idx1 == idx2);

        // Crossover
        Solution parent1 = population[idx1];
        Solution parent2 = population[idx2];
        Solution child = crossover(parent1, parent2, distanceMatrix);

        int roll = localSearchWeightDist(rng);
        if (roll <= 90)
        {
            child = localSearchMemory(child, distanceMatrix);
        }
        else
        {
            std::uniform_int_distribution<int> nhSizeDist(1, distanceMatrix.size() / 2 - 2);
            int neighbourhoodSize = nhSizeDist(rng);
            child = localSearchCandidates(child, distanceMatrix, neighbourhoodSize);
        }
        child.calculateScore(distanceMatrix);
        insertIntoPopulation(population, child, POPULATION_SIZE);
        iterations++;
    }
    std::cout << "Iterations " << iterations << std::endl;
    std::cout << "Runtime " << runtime << std::endl;

    return std::make_pair(population.front(), iterations);
}