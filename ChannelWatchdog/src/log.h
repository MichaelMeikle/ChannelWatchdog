#pragma once
#include "common.h"
#include <string>

class Log
{
	int log_level_;

public:
	Log();
	void Debug(std::string msg);
	void Info(std::string msg);
	void Warning(std::string msg);
	void Error(std::string msg);

	/*** Getters ***/
	int log_level();
	/*** Setters ***/
	void log_level(int log_level);
private:
};