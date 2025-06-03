#ifndef FREESTYLE_H
#define FREESTYLE_H

#include <vector>
#include "point.h"

std::pair<Solution, int> freestyle(const std::vector<std::vector<double>> &distanceMatrix, int timeLimit);

#endif