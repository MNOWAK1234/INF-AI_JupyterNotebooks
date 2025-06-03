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

#define POPULATION_SIZE 20

bool insertIntoPopulation(std::vector<Solution> &population, const Solution &child, size_t MAX_POPULATION_SIZE = 10)
{
    if (population.empty())
    {
        population.push_back(child);
        return true;
    }

    auto it = std::lower_bound(population.begin(), population.end(), child);
    size_t pos = std::distance(population.begin(), it);

    // Too weak to be included
    if (pos == MAX_POPULATION_SIZE)
        return false;

    if (pos == 0)
    {
        if (child != population[0])
        {
            population.insert(it, child);
            if (population.size() > MAX_POPULATION_SIZE)
                population.pop_back();
            return true;
        }
    }
    else
    {
        bool diffPrev = child != population[pos - 1];
        bool diffNext = true;
        if (pos < population.size())
        {
            diffNext = child != population[pos];
        }

        if (diffPrev && diffNext)
        {
            population.insert(it, child);
            if (population.size() > MAX_POPULATION_SIZE)
                population.pop_back();
            return true;
        }
    }

    return false;
}

Solution crossover(const Solution &parent1, const Solution &parent2, const std::vector<std::vector<double>> &distanceMatrix)
{
    Solution child;

    int totalSize = distanceMatrix.size();
    std::vector<bool> visited(totalSize, false);
    int size1 = static_cast<int>(parent1.cycleIndices[0].size());
    int size2 = static_cast<int>(parent1.cycleIndices[1].size());

    // Choose random a, b in [0, size1), c, d in [0, size2)
    int a = std::rand() % size1;
    int b = std::rand() % size1;
    int c = std::rand() % size2;
    int d = std::rand() % size2;

    // Extract chunk from a to b from cycle 0
    if (a <= b)
    {
        child.cycleIndices[0].insert(child.cycleIndices[0].end(), parent1.cycleIndices[0].begin() + a, parent1.cycleIndices[0].begin() + b + 1);
    }
    else
    {
        child.cycleIndices[0].insert(child.cycleIndices[0].end(), parent1.cycleIndices[0].begin() + a, parent1.cycleIndices[0].end());
        child.cycleIndices[0].insert(child.cycleIndices[0].end(), parent1.cycleIndices[0].begin(), parent1.cycleIndices[0].begin() + b + 1);
    }

    // Extract chunk from c to d from cycle 1
    if (c <= d)
    {
        child.cycleIndices[1].insert(child.cycleIndices[1].end(), parent1.cycleIndices[1].begin() + c, parent1.cycleIndices[1].begin() + d + 1);
    }
    else
    {
        child.cycleIndices[1].insert(child.cycleIndices[1].end(), parent1.cycleIndices[1].begin() + c, parent1.cycleIndices[1].end());
        child.cycleIndices[1].insert(child.cycleIndices[1].end(), parent1.cycleIndices[1].begin(), parent1.cycleIndices[1].begin() + d + 1);
    }
    // Mark used points as visited
    for (int i = 0; i < static_cast<int>(child.cycleIndices[0].size()); ++i)
    {
        visited[child.cycleIndices[0][i]] = true;
    }
    for (int i = 0; i < static_cast<int>(child.cycleIndices[1].size()); ++i)
    {
        visited[child.cycleIndices[1][i]] = true;
    }

    // Create what's left from parent2 (keeping order)
    Solution whatsLeft;

    for (int i = 0; i < static_cast<int>(parent2.cycleIndices[0].size()); ++i)
    {
        int point = parent2.cycleIndices[0][i];
        if (!visited[point])
        {
            whatsLeft.cycleIndices[0].push_back(point);
            visited[point] = true;
        }
    }

    for (int i = 0; i < static_cast<int>(parent2.cycleIndices[1].size()); ++i)
    {
        int point = parent2.cycleIndices[1][i];
        if (!visited[point])
        {
            whatsLeft.cycleIndices[1].push_back(point);
            visited[point] = true;
        }
    }

    // Random choice: which whatsLeft cycle merges into child.cycleIndices[0]
    int mergeWith0 = std::rand() % 2; // 0 or 1
    int mergeWith1 = 1 - mergeWith0;
    std::vector<int> mergeWith = {mergeWith0, mergeWith1};

    for (int c = 0; c < 2; c++)
    {
        if (whatsLeft.cycleIndices[mergeWith[c]].empty())
            continue;

        double bestCost = std::numeric_limits<double>::max();
        bool bestIsReversed = false;
        int bestCut = -1;

        int childFront = child.cycleIndices[c].front();
        int childBack = child.cycleIndices[c].back();

        int size = static_cast<int>(whatsLeft.cycleIndices[mergeWith[c]].size());

        for (int cut = 0; cut < size; cut++)
        {
            // Normal direction
            int end1 = whatsLeft.cycleIndices[mergeWith[c]][cut];
            int end2 = whatsLeft.cycleIndices[mergeWith[c]][(cut + 1) % size];
            double costNormal = distanceMatrix[childFront][end1] + distanceMatrix[childBack][end2];

            if (costNormal < bestCost)
            {
                bestCost = costNormal;
                bestIsReversed = false;
                bestCut = cut;
            }

            // Reversed direction
            int reversedEnd1 = end2;
            int reversedEnd2 = end1;
            double costReversed = distanceMatrix[childFront][reversedEnd1] + distanceMatrix[childBack][reversedEnd2];

            if (costReversed < bestCost)
            {
                bestCost = costReversed;
                bestIsReversed = true;
                bestCut = cut;
            }
        }

        // Perform insertion
        std::vector<int> reordered;
        for (int i = 0; i < size; ++i)
        {
            reordered.push_back(whatsLeft.cycleIndices[mergeWith[c]][(bestCut + i) % size]);
        }
        if (bestIsReversed)
        {
            std::reverse(reordered.begin(), reordered.end());
        }

        // Append to the child cycle
        child.cycleIndices[c].insert(child.cycleIndices[c].end(), reordered.begin(), reordered.end());
    }

    // Balance the child cycles if they are not equal in size
    int targetSize = totalSize / 2;

    int fromCycle;
    int toCycle;
    if (static_cast<int>(child.cycleIndices[0].size()) > targetSize)
    {
        fromCycle = 0;
        toCycle = 1;
    }
    else
    {
        fromCycle = 1;
        toCycle = 0;
    }

    while (static_cast<int>(child.cycleIndices[fromCycle].size()) > targetSize)
    {
        double bestDeltaCost = std::numeric_limits<double>::max();
        int bestFromIndex = -1;
        int bestToIndex = -1;

        for (int i = 0; i < static_cast<int>(child.cycleIndices[fromCycle].size()); ++i)
        {
            int pointToMove = child.cycleIndices[fromCycle][i];

            // Compute removal cost from current cycle
            int prevIndex = i - 1;
            if (prevIndex < 0)
            {
                prevIndex = static_cast<int>(child.cycleIndices[fromCycle].size()) - 1;
            }
            int nextIndex = (i + 1) % static_cast<int>(child.cycleIndices[fromCycle].size());

            int prev = child.cycleIndices[fromCycle][prevIndex];
            int next = child.cycleIndices[fromCycle][nextIndex];

            double removalCost = distanceMatrix[prev][pointToMove] + distanceMatrix[pointToMove][next] - distanceMatrix[prev][next];

            // Try insertion into all positions of the other cycle
            for (int j = 0; j <= static_cast<int>(child.cycleIndices[toCycle].size()); ++j)
            {
                int before, after;

                if (j == 0)
                {
                    before = child.cycleIndices[toCycle].back();
                }
                else
                {
                    before = child.cycleIndices[toCycle][j - 1];
                }

                if (j == static_cast<int>(child.cycleIndices[toCycle].size()))
                {
                    after = child.cycleIndices[toCycle].front();
                }
                else
                {
                    after = child.cycleIndices[toCycle][j];
                }

                double insertionCost = distanceMatrix[before][pointToMove] + distanceMatrix[pointToMove][after] - distanceMatrix[before][after];

                double delta = removalCost + insertionCost;
                if (delta < bestDeltaCost)
                {
                    bestDeltaCost = delta;
                    bestFromIndex = i;
                    bestToIndex = j;
                }
            }
        }

        // Move the best point found
        int pointToMove = child.cycleIndices[fromCycle][bestFromIndex];
        child.cycleIndices[fromCycle].erase(child.cycleIndices[fromCycle].begin() + bestFromIndex);
        child.cycleIndices[toCycle].insert(child.cycleIndices[toCycle].begin() + bestToIndex, pointToMove);
    }
    child.updatePointPositions();
    return child;
}

Solution proposedCrossover(const Solution &parent1, const Solution &parent2, const std::vector<std::vector<double>> &distanceMatrix)
{
    // choose parent at random
    int which_parent = std::rand() % 2;
    Solution solution;
    Solution other;

    if (which_parent == 0)
    {
        solution = parent1;
        other = parent2;
    }
    else
    {
        solution = parent2;
        other = parent1;
    }

    // Delete edges are not present in the other parent
    std::vector<bool> toRemoveCycle1(solution.cycleIndices[0].size(), false);
    std::vector<bool> toRemoveCycle2(solution.cycleIndices[1].size(), false);

    for (int i = 1; i < static_cast<int>(solution.cycleIndices[0].size()); i++)
    {
        std::vector<int> sequence = {i - 1, i};
        auto it = std::search(other.cycleIndices[0].begin(), other.cycleIndices[0].end(), sequence.begin(), sequence.end());

        if (it == other.cycleIndices[0].end())
        {
            toRemoveCycle1[i - 1] = true;
            toRemoveCycle1[i] = true;
        }

        it = std::search(other.cycleIndices[1].begin(), other.cycleIndices[1].end(), sequence.begin(), sequence.end());
        if (it == other.cycleIndices[1].end())
        {
            toRemoveCycle1[i - 1] = true;
            toRemoveCycle1[i] = true;
        }
    }

    for (int i = 1; i < static_cast<int>(solution.cycleIndices[1].size()); i++)
    {
        std::vector<int> sequence = {i - 1, i};
        auto it = std::search(other.cycleIndices[0].begin(), other.cycleIndices[0].end(), sequence.begin(), sequence.end());
        if (it == other.cycleIndices[0].end())
        {
            toRemoveCycle2[i - 1] = true;
            toRemoveCycle2[i] = true;
        }

        it = std::search(other.cycleIndices[1].begin(), other.cycleIndices[1].end(), sequence.begin(), sequence.end());
        if (it == other.cycleIndices[1].end())
        {
            toRemoveCycle2[i - 1] = true;
            toRemoveCycle2[i] = true;
        }
    }

    // Create new cycles without the removed points
    std::vector<int> newCycle1;
    for (size_t i = 0; i < solution.cycleIndices[0].size(); i++)
    {
        if (!toRemoveCycle1[i])
            newCycle1.push_back(solution.cycleIndices[0][i]);
    }

    std::vector<int> newCycle2;
    for (size_t i = 0; i < solution.cycleIndices[1].size(); i++)
    {
        if (!toRemoveCycle2[i])
            newCycle2.push_back(solution.cycleIndices[1][i]);
    }

    solution.cycleIndices[0] = newCycle1;
    solution.cycleIndices[1] = newCycle2;

    // Rebuild solution using wighted regret heuristic
    return regretCycleWeighted(solution, distanceMatrix, 1.0, 1.0);
}

std::pair<Solution, int> evolutionaryAlgorithm(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit, bool localSearch)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<Solution> population;
    while (population.size() < POPULATION_SIZE)
    {
        Solution x = randomCycle(distanceMatrix);
        x = localSearchMemory(x, distanceMatrix);
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
        Solution child = proposedCrossover(parent1, parent2, distanceMatrix);

        // Optional local search
        if (localSearch)
        {
            child = localSearchMemory(child, distanceMatrix);
        }
        child.calculateScore(distanceMatrix);
        insertIntoPopulation(population, child, POPULATION_SIZE);
        iterations++;
    }
    std::cout << iterations << std::endl;
    std::cout << runtime << std::endl;

    return std::make_pair(population.front(), iterations);
}