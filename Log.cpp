#include "Log.h"

Log::Log(string filename)
{
	GetLocalTime(&SystemTime);
	file.open(filename.c_str(),ios_base::app);
	file << "#>> " << TIMESTAMP << " <<#" << endl;
}

void Log::Add(string text)
{
	GetLocalTime(&SystemTime);
	file << "#" << TIMESTAMP2 <<">> " << text << endl;
}


Log::~Log()
{
	GetLocalTime(&SystemTime);
	file << "##" << TIMESTAMP <<"##" << endl << endl << endl;
	file.close();
}



const char* cpDayOfWeek(int num)
{
	switch(num)
	{
	case 1:
		return "Monday";
	case 2:
		return "Tuesday";
	case 3:
		return "Wednesday";
	case 4:
		return "Thursday";
	case 5:
		return "Friday";
	case 6:
		return "Saturday";
	case 7:
		return "Sunday";
	}
	return "Shitfuck";
}