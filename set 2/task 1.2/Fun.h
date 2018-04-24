#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <ctime>

bool prepareSequence(std::fstream &file, std::vector <int> &sequence, std::vector <int> &orginal);
bool myCompare(int x, int y);
bool checkGraph(std::vector <int> &sequence);
void generateGraph(std::vector<int> &orginal, std::vector<std::vector<int>> &adjacencyList);
bool comparePair(std::pair<int, int> p1, std::pair<int, int> p2);
void randomizeGraph(std::vector<std::pair<int, int>> &pairs);
void setOrginalPairs(std::vector<std::vector<int>> &adjacencyList, std::vector<std::pair<int, int>> &pairs);
bool findPair(std::vector<std::pair<int, int>> &pairs, int x, int y);
void deletePair(std::vector<std::pair<int, int>> &pairs, int x, int y);