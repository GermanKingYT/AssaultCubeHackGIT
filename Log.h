#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <Windows.h>
using namespace std;

#define WIN32_LEAN_AND_MEAN

#pragma once


const char* cpDayOfWeek(int num);


class Log
{
private:
	SYSTEMTIME SystemTime;
	ofstream file;
public:
	Log(string filenameToOpen);
	void Add(string line);
	~Log();
};








#define TIMESTAMP  cpDayOfWeek(SystemTime.wDayOfWeek) << ", " << SystemTime.wDay << "." << SystemTime.wMonth <<  "." << SystemTime.wYear << ", " << SystemTime.wHour << ":" << SystemTime.wMinute << ":" << SystemTime.wSecond << ":" << SystemTime.wMilliseconds
#define TIMESTAMP2 SystemTime.wHour << ":" << SystemTime.wMinute << ":" << SystemTime.wSecond << ":" << SystemTime.wMilliseconds