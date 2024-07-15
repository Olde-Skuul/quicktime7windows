#pragma once
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include <MacTypes.h>
#include <QuickTimeComponents.h>

using namespace std;

typedef map<string, OSErr> errorDict;
typedef map<string, string> commands;

template<typename T>
void print(T toprint)
{
	std::cout << toprint << std::endl;
}

namespace vectortools {
template<typename T>
bool contains(const vector<T>& vec, const T& item)
{
	return (std::find(vec.begin(), vec.end(), item) != vec.end());
}

template<typename T>
bool contains(const commands& inmap, const T& item)
{
	return (inmap.find(item) != inmap.end());
}
}

namespace stringtools {
vector<string> split(const string& instring, const string& delim);
}

namespace Disney {
extern const map<int, int> Disney24ChConfig;
}

class argHandler {
private:
	// these singlecmds will appear in the tasks attribute (string vector)
	const vector<string> singlecmds{"flagaudio", "setTC", "me6ch", "disney24"};
	vector<string> all;
	commands params;

	void parsePath();
	void parseTC();
	void parsechannels();
	void parsecmds();
	void parseFPS();

public:
	vector<string> tasks;
	array<int, 2> channelRange{};
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