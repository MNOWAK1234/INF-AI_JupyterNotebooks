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
#include "optimization.h"

void enqueueImprovingVertexMoves(
    const std::vector<int> &pointsToUpdate,
    const Solution &solution,
    const std::vector<std::vector<double>> &distanceMatrix,
    const std::vector<int> &cycleSizes,
    std::priority_queue<Move> &pq,
    double epsilon)
{
    for (int updatedPoint : pointsToUpdate)
    {
        auto [cycle, position] = solution.getPointPosition(updatedPoint);
        int otherCycle = 1 - cycle;

        for (int j = 0; j < cycleSizes[otherCycle]; j++)
        {
            double delta = 0.0;

            int otherPoint = solution.cycleIndices[otherCycle][j];

            int newNext1 = solution.cycleIndices[cycle][(position + 1) % cycleSizes[cycle]];
            int newPrev1 = solution.cycleIndices[cycle][(position - 1 + cycleSizes[cycle]) % cycleSizes[cycle]];

            int newNext2 = solution.cycleIndices[otherCycle][(j + 1) % cycleSizes[otherCycle]];
            int newPrev2 = solution.cycleIndices[otherCycle][(j - 1 + cycleSizes[otherCycle]) % cycleSizes[otherCycle]];

            delta -= distanceMatrix[updatedPoint][newNext1];
            delta -= distanceMatrix[updatedPoint][newPrev1];
            delta -= distanceMatrix[otherPoint][newNext2];
            delta -= distanceMatrix[otherPoint][newPrev2];

            delta += distanceMatrix[updatedPoint][newNext2];
            delta += distanceMatrix[updatedPoint][newPrev2];
            delta += distanceMatrix[otherPoint][newNext1];
            delta += distanceMatrix[otherPoint][newPrev1];

            if (delta < -epsilon)
            {
                pq.push(VertexMove(
                    {newPrev1, updatedPoint, newNext1},
                    {newPrev2, otherPoint, newNext2},
                    delta));
            }
        }
    }
}

void enqueueImprovingEdgeMoves(
    const std::vector<Cut> &cutsToUpdate,
    const Solution &solution,
    const std::vector<std::vector<double>> &distanceMatrix,
    const std::vector<int> &cycleSizes,
    std::priority_queue<Move> &pq,
    double epsilon)
{
    for (int i = 0; i < cycleSizes[0]; i++)
    {
        for (const Cut &cut : cutsToUpdate)
        {
            auto [cycleOfCut, positionOfCut] = solution.getPointPosition(cut.prev);

            // Skip reversing the entire cycle
            if (i == positionOfCut ||
                (i + 1) % cycleSizes[cycleOfCut] == positionOfCut ||
                (positionOfCut + 1) % cycleSizes[cycleOfCut] == i)
            {
                continue;
            }

            int point1 = solution.cycleIndices[cycleOfCut][i];
            int next1 = solution.cycleIndices[cycleOfCut][(i + 1) % cycleSizes[cycleOfCut]];
            int point2 = solution.cycleIndices[cycleOfCut][positionOfCut];
            int next2 = solution.cycleIndices[cycleOfCut][(positionOfCut + 1) % cycleSizes[cycleOfCut]];

            double delta = 0.0;
            delta -= distanceMatrix[point1][next1];
            delta -= distanceMatrix[point2][next2];
            delta += distanceMatrix[next1][next2];
            delta += distanceMatrix[point1][point2];

            if (delta < -epsilon)
            {
                pq.push(EdgeMove({point1, next1}, {point2, next2}, delta));
            }
        }
    }
}

Solution localSearchMemory(Solution solution, const std::vector<std::vector<double>> &distanceMatrix)
{
    std::priority_queue<Move> pq;                                           // Push all the legal moves to the queue
    const double epsilon = 1e-6;                                            // Avoid numerical errors
    int pointsInCycle1 = static_cast<int>(solution.cycleIndices[0].size()); // Size of first cycle
    int pointsInCycle2 = static_cast<int>(solution.cycleIndices[1].size()); // Size of second cycle
    std::vector<int> cycleSizes = {pointsInCycle1, pointsInCycle2};

    int point1;
    int point2;
    int cycleOfPoint1 = 0;
    int cycleOfPoint2 = 1;
    double delta;

    // Add vertex swap between 2 cycles
    // For some reason it should always be a legal move
    for (int i = 0; i < pointsInCycle1; ++i)
    {
        for (int j = 0; j < pointsInCycle2; ++j)
        {
            delta = 0;
            // Get all the neighbours of point 1
            int next1;
            int prev1;
            point1 = solution.cycleIndices[cycleOfPoint1][i % pointsInCycle1];
            next1 = solution.cycleIndices[cycleOfPoint1][(i + 1) % cycleSizes[cycleOfPoint1]];
            prev1 = solution.cycleIndices[cycleOfPoint1][(i - 1 + cycleSizes[cycleOfPoint1]) % cycleSizes[cycleOfPoint1]];
            // Get all the neighbours of point 2
            int next2;
            int prev2;
            point2 = solution.cycleIndices[cycleOfPoint2][j % pointsInCycle2];
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
            if (delta < -epsilon)
            {
                pq.push(VertexMove({prev1, point1, next1}, {prev2, point2, next2}, delta));
            }
        }
    }
    // Add edge exchange
    for (size_t c = 0; c < 2; ++c)
    {
        // Loop over each pair of points in the solution
        for (int i = 0; i < cycleSizes[c]; ++i)
        {
            for (int j = 0; j < cycleSizes[c]; ++j) // Avoid reversing whole cycle
            {
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

                if (delta < -epsilon)
                {
                    // This time we only do cuts (prev1 X point1) and (point2 X next2)
                    pq.push(EdgeMove({prev1, point1}, {point2, next2}, delta));
                }
            }
        }
    }
    int no = 0;
    while (!pq.empty())
    {
        Move best = pq.top();
        pq.pop();

        if (best.type == moveType::VERTEX)
        {
            // Get current positions of the points in the cycles
            auto [cycle1, pos1] = solution.getPointPosition(best.neighbourhood1.mid);
            auto [cycle2, pos2] = solution.getPointPosition(best.neighbourhood2.mid);
            point1 = best.neighbourhood1.mid;
            point2 = best.neighbourhood2.mid;

            // Ensure the points are in separate cycles
            if (cycle1 != cycle2)
            {
                // Get the prev and next indices for both points
                int prev1 = solution.cycleIndices[cycle1][(pos1 - 1 + cycleSizes[cycle1]) % cycleSizes[cycle1]];
                int next1 = solution.cycleIndices[cycle1][(pos1 + 1) % cycleSizes[cycle1]];
                int prev2 = solution.cycleIndices[cycle2][(pos2 - 1 + cycleSizes[cycle2]) % cycleSizes[cycle2]];
                int next2 = solution.cycleIndices[cycle2][(pos2 + 1) % cycleSizes[cycle2]];

                VertexNeighbourhood neighbourhood1(prev1, point1, next1);
                VertexNeighbourhood neighbourhood2(prev2, point2, next2);

                // Check legality by comparing prevs and nexts, including reversed cases
                if ((neighbourhood1 == best.neighbourhood1 && neighbourhood2 == best.neighbourhood2) || (neighbourhood2 == best.neighbourhood1 && neighbourhood1 == best.neighbourhood2))
                {
                    no++;
                    // std::cout << "Move vertices " << no << ": " << best.neighbourhood1.mid << " " << best.neighbourhood2.mid << " score " << best.delta << "   ---------   ";
                    // std::cout << "neighbours1 " << best.neighbourhood1.prev << " " << best.neighbourhood1.next << " neighbours2 " << best.neighbourhood1.prev << " " << best.neighbourhood2.next << std::endl;

                    //  Swap the points between the two cycles
                    std::swap(solution.cycleIndices[cycle1][pos1], solution.cycleIndices[cycle2][pos2]);
                    std::swap(solution.pointPositions[point1], solution.pointPositions[point2]);
                    // And new potential moves
                    // That is both swapped points and their neighbours
                    std::vector<int> pointsToUpdate = {point1, prev1, next1, point2, prev2, next2};
                    Cut cut1 = {prev1, point1};
                    Cut cut2 = {point1, next1};
                    Cut cut3 = {prev2, point2};
                    Cut cut4 = {point2, next2};
                    std::vector<Cut> cutsToUpdate = {cut1, cut2, cut3, cut4};
                    enqueueImprovingEdgeMoves(cutsToUpdate, solution, distanceMatrix, cycleSizes, pq, epsilon);
                    enqueueImprovingVertexMoves(pointsToUpdate, solution, distanceMatrix, cycleSizes, pq, epsilon);
                }
            }
        }
        else
        {
            // Get current positions of the points in the cycles
            auto [cycle1, pos1] = solution.getPointPosition(best.cut1.prev);
            auto [cycle2, pos2] = solution.getPointPosition(best.cut2.prev);
            int c = cycle1; // Both cycles are the same
            // Ensure position 1 is before position 2
            if (pos1 > pos2)
            {
                std::swap(pos1, pos2);
                std::swap(best.cut1, best.cut2);
            }
            point1 = solution.cycleIndices[c][pos1];
            point2 = solution.cycleIndices[c][pos2];
            //  Get the prev and next indices for both points
            int prev1 = solution.cycleIndices[c][(pos1 - 1 + cycleSizes[c]) % cycleSizes[c]];
            int next1 = solution.cycleIndices[c][(pos1 + 1) % cycleSizes[c]];
            int prev2 = solution.cycleIndices[c][(pos2 - 1 + cycleSizes[c]) % cycleSizes[c]];
            int next2 = solution.cycleIndices[c][(pos2 + 1) % cycleSizes[c]];

            Cut cut1;
            Cut cut2;
            int innerStart, innerEnd;
            std::vector<int> pointsToUpdate = {point1, point2};
            if (prev1 == best.cut1.next)
            {
                cut1 = Cut(prev1, point1);
                innerStart = pos1;
                pointsToUpdate.push_back(best.cut1.next);
            }
            else if (next1 == best.cut1.next)
            {
                cut1 = Cut(point1, next1);
                innerStart = (pos1 + 1) % cycleSizes[c];
                pointsToUpdate.push_back(best.cut1.next);
            }
            if (prev2 == best.cut2.next)
            {
                cut2 = Cut(prev2, point2);
                innerEnd = (pos2 - 1 + cycleSizes[c]) % cycleSizes[c];
                pointsToUpdate.push_back(best.cut2.next);
            }
            else if (next2 == best.cut2.next)
            {
                cut2 = Cut(point2, next2);
                innerEnd = pos2;
                pointsToUpdate.push_back(best.cut2.next);
            }
            if ((cut1.isSameDirectionAs(best.cut1) && cut2.isSameDirectionAs(best.cut2)) ||
                (cut1.isReversedOf(best.cut1) && cut2.isReversedOf(best.cut2)))
            {
                no++;
                std::reverse(solution.cycleIndices[c].begin() + innerStart, solution.cycleIndices[c].begin() + innerEnd + 1);
                solution.updatePointPositions();
                // Add new legal moves
                int outerIterator = (innerEnd + 1) % cycleSizes[c];
                while (outerIterator != innerStart)
                {
                    int innerIterator = innerStart % cycleSizes[c]; // skip the first cut (it has to update whole array)
                    while (innerIterator != innerEnd)               // skip the last cut for the same reason
                    {
                        // Skip reversing the entire cycle
                        if (outerIterator == innerIterator || (innerIterator + 1) % cycleSizes[c] == outerIterator || (outerIterator + 1) % cycleSizes[c] == innerIterator)
                        {
                            innerIterator++;
                            innerIterator = innerIterator % cycleSizes[c];
                            continue;
                        }

                        double delta = 0.0;
                        int next1, next2;
                        point1 = solution.cycleIndices[c][outerIterator];
                        next1 = solution.cycleIndices[c][(outerIterator + 1) % cycleSizes[c]];
                        point2 = solution.cycleIndices[c][innerIterator];
                        next2 = solution.cycleIndices[c][(innerIterator + 1) % cycleSizes[c]];

                        delta -= distanceMatrix[point1][next1];
                        delta -= distanceMatrix[point2][next2];
                        delta += distanceMatrix[next1][next2];
                        delta += distanceMatrix[point1][point2];
                        if (delta < -epsilon)
                        {
                            pq.push(EdgeMove({point1, next1}, {point2, next2}, delta));
                        }

                        innerIterator++;
                        innerIterator = innerIterator % cycleSizes[c];
                    }
                    outerIterator++;
                    outerIterator = outerIterator % cycleSizes[c];
                }
                Cut afterExchange1 = {best.cut1.prev, best.cut2.prev};
                Cut afterExchange2 = {best.cut1.next, best.cut2.next};
                std::vector<Cut> cutsToUpdate = {afterExchange1, afterExchange2};
                enqueueImprovingEdgeMoves(cutsToUpdate, solution, distanceMatrix, cycleSizes, pq, epsilon);
                enqueueImprovingVertexMoves(pointsToUpdate, solution, distanceMatrix, cycleSizes, pq, epsilon);
            }
        }
    }

    return solution;
}

std::vector<std::vector<int>> computeNearestneighbours(
    const std::vector<std::vector<double>> &distanceMatrix,
    int n = 10)
{
    size_t numberOfPoints = distanceMatrix.size();
    std::vector<std::vector<int>> nearestneighbours(numberOfPoints);

    for (size_t i = 0; i < numberOfPoints; ++i)
    {
        std::vector<std::pair<double, int>> distances;

        for (size_t j = 0; j < numberOfPoints; ++j)
        {
            if (i == j)
                continue;
            distances.emplace_back(distanceMatrix[i][j], j);
        }

        std::partial_sort(
            distances.begin(),
            distances.begin() + std::min(n, (int)distances.size()),
            distances.end());

        for (int k = 0; k < std::min(n, (int)distances.size()); ++k)
        {
            nearestneighbours[i].push_back(distances[k].second);
        }
    }

    return nearestneighbours;
}

Solution localSearchCandidates(Solution solution, const std::vector<std::vector<double>> &distanceMatrix, int neighbourhoodSize)
{
    const double epsilon = 1e-6;                                            // Avoid numerical errors
    int pointsInCycle1 = static_cast<int>(solution.cycleIndices[0].size()); // Size of first cycle
    int pointsInCycle2 = static_cast<int>(solution.cycleIndices[1].size()); // Size of second cycle
    neighbourhoodSize = std::max(1, std::min(neighbourhoodSize, std::min(pointsInCycle1, pointsInCycle2) - 2));
    std::vector<int> cycleSizes = {pointsInCycle1, pointsInCycle2};
    int numberOfNearestPoints = 10;

    auto nearestneighbours = computeNearestneighbours(distanceMatrix, numberOfNearestPoints);

    bool improvement = true;
    while (improvement)
    {
        improvement = false;
        double minDelta = 0.0;
        int whichPoint;
        int insertedNeighbour;
        int whoStays;
        int whichSide;
        int whichSideOther;
        int whichLength;
        bool sameCycle = true;
        double oldCost, newCost;

        // For each cycle
        for (int c = 0; c < 2; c++)
        {
            // Check each point in cycle
            for (int i = 0; i < cycleSizes[c]; i++)
            {
                int currentPoint = solution.cycleIndices[c][i];

                // And its nearest neighbours
                for (int j = 0; j < numberOfNearestPoints; j++)
                {
                    int neighbour = nearestneighbours[currentPoint][j];
                    auto [neighbourCycle, neighbourPosition] = solution.getPointPosition(neighbour);
                    double delta = 0.0;

                    if (neighbourCycle == c)
                    {
                        if ((i + 1) % cycleSizes[c] == neighbourPosition || (neighbourPosition + 1) % cycleSizes[c] == i)
                        {
                            continue; // Skip if the point is already there
                        }
                        // Check both +1 and -1 position
                        std::vector<int> sides = {-1, 1};
                        for (const int side : sides)
                        {
                            int insertingPosition = (i + side + cycleSizes[c]) % cycleSizes[c];
                            int otherCurrentlyThere = solution.cycleIndices[c][insertingPosition];
                            newCost = distanceMatrix[currentPoint][neighbour];
                            oldCost = distanceMatrix[currentPoint][otherCurrentlyThere];
                            double insertNeighbourDelta = newCost - oldCost;
                            int indexConnectedToNeighbour = (neighbourPosition + side + cycleSizes[c]) % cycleSizes[c];
                            int connectedToNeighbour = solution.cycleIndices[c][indexConnectedToNeighbour];
                            newCost = distanceMatrix[otherCurrentlyThere][connectedToNeighbour];
                            oldCost = distanceMatrix[neighbour][connectedToNeighbour];
                            double secondEdgeDelta = newCost - oldCost;
                            delta = insertNeighbourDelta + secondEdgeDelta;
                            if (delta < minDelta - epsilon)
                            {
                                minDelta = delta;
                                improvement = true;
                                whichPoint = currentPoint;
                                insertedNeighbour = neighbour;
                                whichSide = side;
                                sameCycle = true;
                            }
                        }
                    }
                    else
                    {
                        // Check both +1 and -1 position
                        std::vector<int> sides = {-1, 1};
                        for (const int side : sides)
                        {
                            // Check both +1 and -1 position for the other cycle
                            std::vector<int> sidesOther = {-1, 1};
                            for (const int sideOther : sidesOther)
                            {
                                // Check all possible lengths
                                for (int l = 1; l <= neighbourCycle; l++)
                                {
                                    int indexRemoved1 = (i + side * l + cycleSizes[c]) % cycleSizes[c];
                                    int pointRemoved1 = solution.cycleIndices[c][indexRemoved1];
                                    int indexRemoved2 = (i + side + cycleSizes[c]) % cycleSizes[c];
                                    int pointRemoved2 = solution.cycleIndices[c][indexRemoved2];
                                    int indexToJoin = (i + side * (l + 1) + cycleSizes[c]) % cycleSizes[c];
                                    int pointToJoin = solution.cycleIndices[c][indexToJoin];

                                    int indexStayInCycleNeighbour1 = (neighbourPosition - sideOther + cycleSizes[neighbourCycle]) % cycleSizes[neighbourCycle];
                                    int pointStayInCycleNeighbour1 = solution.cycleIndices[neighbourCycle][indexStayInCycleNeighbour1];
                                    int indexEndSegmentNeighbour = (indexStayInCycleNeighbour1 + sideOther * l + cycleSizes[neighbourCycle]) % cycleSizes[neighbourCycle];
                                    int pointEndSegmentNeighbour = solution.cycleIndices[neighbourCycle][indexEndSegmentNeighbour];
                                    int indexStayInCycleNeighbour2 = (indexStayInCycleNeighbour1 + sideOther * (l + 1) + cycleSizes[neighbourCycle]) % cycleSizes[neighbourCycle];
                                    int pointStayInCycleNeighbour2 = solution.cycleIndices[neighbourCycle][indexStayInCycleNeighbour2];

                                    oldCost = distanceMatrix[currentPoint][pointRemoved2] + distanceMatrix[pointToJoin][pointRemoved1];
                                    newCost = distanceMatrix[currentPoint][neighbour] + distanceMatrix[pointToJoin][pointEndSegmentNeighbour];
                                    double currentCycleDelta = newCost - oldCost;
                                    oldCost = distanceMatrix[pointStayInCycleNeighbour1][neighbour] + distanceMatrix[pointStayInCycleNeighbour2][pointEndSegmentNeighbour];
                                    newCost = distanceMatrix[pointStayInCycleNeighbour1][pointRemoved2] + distanceMatrix[pointStayInCycleNeighbour2][pointRemoved1];
                                    double neighbourCycleDelta = newCost - oldCost;
                                    delta = currentCycleDelta + neighbourCycleDelta;
                                    if (delta < minDelta - epsilon)
                                    {
                                        minDelta = delta;
                                        improvement = true;
                                        whichPoint = currentPoint;
                                        insertedNeighbour = neighbour;
                                        whoStays = pointStayInCycleNeighbour1;
                                        whichSide = side;
                                        whichSideOther = sideOther;
                                        whichLength = l;
                                        sameCycle = false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (improvement)
        {
            if (sameCycle == true)
            {
                // It's basically the same as edge exchange (rotation)
                auto [cycle1, smallerPos] = solution.getPointPosition(whichPoint);
                auto [cycle2, greaterPos] = solution.getPointPosition(insertedNeighbour);
                int c = cycle1; // Both cycles are the same
                // Ensure position 1 is before position 2
                if (smallerPos > greaterPos)
                {
                    std::swap(smallerPos, greaterPos);
                }
                int innerStart = smallerPos;
                int innerEnd = greaterPos;
                if (whichSide == 1)
                {
                    innerStart = smallerPos + 1;
                    innerEnd = greaterPos;
                }
                else // -1
                {
                    innerStart = smallerPos;
                    innerEnd = greaterPos - 1;
                }

                std::reverse(solution.cycleIndices[c].begin() + innerStart, solution.cycleIndices[c].begin() + innerEnd + 1);
                solution.updatePointPositions();
            }
            else
            {
                auto [whichCycle, stayPosition] = solution.getPointPosition(whichPoint);
                auto [otherCycle, otherStayPosition] = solution.getPointPosition(whoStays);

                // Replace segments of length whichLength
                for (int l = 1; l <= whichLength; l++)
                {
                    int index1 = (stayPosition + (whichSide * l) + cycleSizes[whichCycle]) % cycleSizes[whichCycle];
                    int index2 = (otherStayPosition + (whichSideOther * l) + cycleSizes[otherCycle]) % cycleSizes[otherCycle];
                    std::swap(solution.pointPositions[solution.cycleIndices[whichCycle][index1]], solution.pointPositions[solution.cycleIndices[otherCycle][index2]]);
                    std::swap(solution.cycleIndices[whichCycle][index1], solution.cycleIndices[otherCycle][index2]);
                }
            }
        }
    }

    return solution;
}