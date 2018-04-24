#pragma once
#include "vcruntime_exception.h"
#include <iostream>
class myException :
	public std::exception
{
public:
	const char* what()const {
		return err_name.c_str();
	}
	myException(std::string error):err_name("Exception: "+error){}
	~myException(){}
private:
	std::string err_name;
};

