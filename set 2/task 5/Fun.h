#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <string>
#include <map>

void generateGraphFullRandom(int k, int w);
void connect(std::vector<std::vector<int>> &vertexes, int v1, int v2);
bool areConnected(std::vector<std::vector<int>> &vertexes, int v1, int v2);
void saveToFile(std::vector<std::vector<int>> &vertexes);
int lowestBucket(std::vector<std::vector<int>> &bucket);

