#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>

#if !defined(__MACH__)
#include <MacTypes.h>
#include <QuickTimeComponents.h>
#else
#include <QuickTime/QuickTime.h>
#endif

using namespace std;

typedef map<string, OSErr> errorDict;
typedef map<string, string> commands;

template<typename T>
void print(T toprint)
{
	std::cout << toprint << std::endl;
}

namespace stringtools {
vector<string> split(const string& instring, const string& delim);
}

namespace Disney {
extern int Disney24ChConfig(int iIndex);
}

namespace vectortools {
extern bool contains(const vector<string>& vecx, const string& xitem);
}

class argHandler {
private:
	vector<string> all;
	commands params;

	void parsePath();
	void parseTC();
	void parsechannels();
	void parsecmds();
	void parseFPS();

public:
	vector<string> tasks;
	int m_channelRange[2];
	int numOfTracks;
	string fileURL;
	TimeCodeRecord TCR;
	TimeCodeDef TCD;
	float fps;

	argHandler(int& argc, char* argv[]);
	commands getparams() const
	{
		return this->params;
	}
};
