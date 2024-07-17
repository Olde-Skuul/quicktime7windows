#include "general.h"
#include <math.h>
using namespace std;

static const float SUPPORTED_FPS[] = {
	24.f, 23.98f, 23.976f, 25.f, 30.f, 29.97f, 60.f, 59.94f};

// these singlecmds will appear in the tasks attribute (string vector)
const char* singlecmds[] = {"flagaudio", "setTC", "me6ch", "disney24"};

namespace vectortools {
static bool contains(const commands& inmap, const string& item)
{
	return const_cast<commands&>(inmap).find(item) != inmap.end();
}
}

void argHandler::parsePath()
{
	FILE* file;
	file = fopen(this->params["path"].c_str(), "r");
	if (file != NULL) {
		fclose(file);
		this->fileURL = this->params["path"];
	} else {
		cout << "Provided path does not appear to be a valid file: "
			 << this->params["path"].c_str() << "\n";
		exit(1);
	}
}

void argHandler::parseTC()
{
	vector<string> tcstr = stringtools::split(this->params["tc"], ":");

	if (tcstr.size() != 4) {
		print("Improperly formatted '-tc' parameter");
		exit(1);
	}

	this->TCR.t.hours = (UInt8)atoi(tcstr[0].c_str());
	this->TCR.t.minutes = (UInt8)atoi(tcstr[1].c_str());
	this->TCR.t.seconds = (UInt8)atoi(tcstr[2].c_str());
	this->TCR.t.frames = (UInt8)atoi(tcstr[3].c_str());
}

void argHandler::parseFPS()
{
	try {
		this->fps = static_cast<float>(atof(this->params["fps"].c_str()));
	} catch (const exception& e) {
		print(e.what());
		exit(1);
	}

	int i = 0;
	do {
		if (this->fps == SUPPORTED_FPS[i]) {
			break;
		}
	} while (++i < (sizeof(SUPPORTED_FPS) / sizeof(SUPPORTED_FPS[0])));

	if (i == (sizeof(SUPPORTED_FPS) / sizeof(SUPPORTED_FPS[0]))) {
		cout << "Unsupported framerate: " << this->fps << endl;
		exit(1);
	}

	double fpsint;
	double fpsRemainder = modf(this->fps, &fpsint);

	int divisor;
	if (fpsRemainder == 0) {
		divisor = 1000;
	} else {
		divisor = 1001;
		fpsint = fpsint + 1;
	}

	this->TCD.flags = 0;
	this->TCD.fTimeScale = ((UInt8)fpsint) * 1000;
	this->TCD.frameDuration = divisor;
	this->TCD.numFrames = (UInt8)fpsint;
	this->TCD.padding = 0;
}

void argHandler::parsechannels()
{
	vector<string> channelstr =
		stringtools::split(this->params["channels"], ",");

	if (channelstr.size() != 2) {
		print("Incorrect number of channels supplied in '-channel' parameter");
		exit(1);
	}

	this->m_channelRange[0] = atoi(channelstr[0].c_str());
	this->m_channelRange[1] = atoi(channelstr[1].c_str());

	if (this->m_channelRange[0] < 1 || this->m_channelRange[1] < 1 ||
		this->m_channelRange[1] < this->m_channelRange[0]) {
		print(
			"Incorrect input channels. Input channels cannot be less than 1 and the second channel must be larger than the first");
		exit(1);
	}

	this->numOfTracks = (this->m_channelRange[1] - this->m_channelRange[0]) + 1;

	if (this->numOfTracks != 2 && this->numOfTracks != 6 &&
		this->numOfTracks != 8 && this->numOfTracks != 24) {
		print("Total number of tracks is not 2, 6, 8, or 24");
		exit(1);
	}
}

void argHandler::parsecmds()
{
	string cmd;
	unsigned int counter = 0;
	for (vector<string>::iterator i = this->all.begin(); i != this->all.end();
		 ++i) {
		if ((*i)[0] == '-') {
			// Removes '-'
			cmd = (*i).substr(1, ((*i).size() - 1));

			int j = 0;
			do {
				if (!strcmp(cmd.c_str(), singlecmds[j])) {
					break;
				}
			} while (++j < (sizeof(singlecmds) / sizeof(singlecmds[0])));

			if (j != (sizeof(singlecmds) / sizeof(singlecmds[0]))) {
				this->tasks.push_back(cmd);
			} else if (counter + 1 < this->all.size()) {
				if (this->all[counter + 1][0] != '-') {

					this->params[cmd] = this->all[counter + 1];
				} else {
					cout << "Empty command parameter for " << (*i).c_str()
						 << endl;
					exit(1);
				}
			} else {
				cout << "Empty command parameter for " << (*i).c_str() << endl;
				exit(1);
			}
		}
		++counter;
	}
}

argHandler::argHandler(int& argc, char* argv[])
{
	m_channelRange[0] = 0;
	m_channelRange[1] = 0;

	if (argc < 3) {
		print("Not enough arguments");
		exit(1);
	}

	for (int i = 0; i < argc; ++i) {
		this->all.push_back(argv[i]);
	}

	this->parsecmds();

	if (vectortools::contains(this->params, string("path"))) {
		this->parsePath();
	} else {
		print("No input path specified");
		exit(1);
	}

	if (vectortools::contains(this->tasks, (string) "flagaudio")) {
		if (vectortools::contains(this->params, (string) "channels"))
			this->parsechannels();
		else {
			print("No channel range specified");
			exit(1);
		}
	}

	if (vectortools::contains(this->tasks, (string) "setTC")) {
		if (vectortools::contains(this->params, (string) "fps"))
			this->parseFPS();
		else {
			print("No FPS specified");
			exit(1);
		}

		if (vectortools::contains(this->params, (string) "tc"))
			this->parseTC();
		else {
			print("No TC specified");
			exit(1);
		}
	}
}
