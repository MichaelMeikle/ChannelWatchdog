#pragma once
#include "common.h"
#include <thread>
#include <string>
#define LOG_NAME "Watchdog"
class Log
{
	int log_level_;
	static int log_id_;

public:
	Log();
	Log(int log_level);
	void Debug(std::string msg);
	void Info(std::string msg);
	void Warning(std::string msg);
	void Error(std::string msg);
	/* Prints msg to TeamSpeak server chat */
	void PrintMsg(uint64 serverConnectionHandlerID, std::string msg);

	/*** Getters ***/
	int log_level();
	/*** Setters ***/
	void log_level(int log_level);
private:
};

struct Timer
{
	std::string funcName; //For identification
	std::chrono::time_point<std::chrono::steady_clock> start, end; // Timepoints for begining and end of execution 
	std::chrono::duration<float> duration; //Time that it takes between use
	Log console;
	Timer()
	{
		funcName = "";
		start = std::chrono::high_resolution_clock::now();
	}
	Timer(std::string funcName)
	{
		this->funcName = funcName;
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		float ms = duration.count() * 1000.0f;
		console.Debug("'" + funcName + "' Timer took " + std::to_string(static_cast<double>(ms))); //Console out
	}
	void setFuncName(std::string funcName)
	{
		this->funcName = funcName;
	}
};

struct QLog
{
	Timer runtime;
	Log console;
	std::string sMsg, description;
	QLog(std::string sMsg, std::string description)
	{
		this->sMsg = sMsg; this->description = description; this->runtime.setFuncName(description);
		console.Debug("[" + description + "] " + sMsg);
	}
	~QLog()
	{
		console.Debug("[" + description + "] Action completed");
	}
};