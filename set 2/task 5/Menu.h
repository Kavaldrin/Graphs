#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include "Fun.h"

class Menu
{
public:
	Menu(){}
	~Menu(){}
	void start();
	int getK() { return m_k; }
	int getV() { return m_v; }
	int getChoice(){ return m_choice; }

private:
	int m_k;
	int m_v;
	int m_choice;
};

