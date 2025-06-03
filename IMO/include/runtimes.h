#ifndef RUNTIMES_H
#define RUNTIMES_H

#include <vector>
#include "point.h"

int getTimeLimit(int iterations, std::vector<std::vector<double>> &distanceMatrix);

void lab1(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab2(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab3(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab4(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab5(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab5Heuristic(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab5Evolutionary(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void lab6(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

void convexTestRuntime(std::vector<Point> &points, std::vector<std::vector<double>> &distanceMatrix);

#endif