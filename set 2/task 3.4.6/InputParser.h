#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include "Graph.h"
#include "myException.h"
#include <string>
#include <sstream>
#include <algorithm>


class InputParser
{
public:
	InputParser(std::string fileName);
	std::shared_ptr<Graph> getGraph() { return ptr; }
	~InputParser();
private:
	std::fstream file;
	std::shared_ptr<Graph> ptr;
};

