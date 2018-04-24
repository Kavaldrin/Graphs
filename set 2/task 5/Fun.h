#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <string>
#include <map>

void generateKGraphConnectivity(int k, int w);
void generateGraphFullRandom(int k, int w);
bool leftVertexes(std::map<int, int> &neigh,int k);
void randomizeGraph(std::vector<std::vector<int>> &vertexes);
void connect(std::vector<std::vector<int>> &vertexes, int v1, int v2);
void disconnect(std::vector<std::vector<int>> &vertexes, int v1, int v2);
bool areConnected(std::vector<std::vector<int>> &vertexes, int v1, int v2);
void saveToFile(std::vector<std::vector<int>> &vertexes);
