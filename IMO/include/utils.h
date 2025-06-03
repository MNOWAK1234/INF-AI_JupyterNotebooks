#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "solution.h"
#include "point.h"

std::vector<Point> loadPointsFromFile(const std::string &filepath);

std::vector<std::vector<double>> createdistanceMatrix(const std::vector<Point> &points);

void plotPoints(const std::vector<Point> &points);

void displayDistanceMatrix(const std::vector<std::vector<double>> &distanceMatrix);

void plotSolution(Solution &solution, const std::vector<Point> &points, const std::vector<std::vector<double>> &distanceMatrix, const std::string &title);

double mean(const std::vector<double>& v);

double correlation(const std::vector<double>& x, const std::vector<double>& y);

void plotSimilarity(std::vector<Solution> &solutions, std::vector<double> &similarities, const std::vector<std::vector<double>> &distanceMatrix, const std::string &title);

#endif