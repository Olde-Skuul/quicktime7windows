/***************************************

	Command line argument handler

***************************************/

#include "arghandler.h"
#include "stringtools.h"
#include "vectortools.h"

using namespace std;

static const float SUPPORTED_FPS[] = {
	24.f, 23.98f, 23.976f, 25.f, 30.f, 29.97f, 60.f, 59.94f};

// these singlecmds will appear in the m_Tasks attribute (string vector)
static const char* singlecmds[] = {"flagaudio", "setTC", "me6ch", "disney24"};

/*! ************************************

	\brief Process the "path" parameter

	Parse the path with "path c:\\Foobar.mov"

***************************************/

void argHandler::parsePath(void)
{
	FILE* file = fopen(m_Parameters["path"].c_str(), "r");

	if (file != NULL) {
		fclose(file);
		m_PathName = m_Parameters["path"];

	} else {
		cout << "Provided path does not appear to be a valid file: "
			 << m_Parameters["path"].c_str() << "\n";
		exit(1);
	}
}

/*! ************************************

	\brief Process the "tc" parameter

	Extract the Quicktime TimeCode with the command
	tc 0:1:2:3

***************************************/

void argHandler::parseTC(void)
{
	vector<string> tcstr = stringtools::split(m_Parameters["tc"], ":");

	if (tcstr.size() != 4) {
		print(
			"Improperly formatted '-tc' parameter, needs 4 values\n"
			"separated by colons");
		exit(1);
	}

	m_TimeCodeRecord.t.hours = (UInt8)atoi(tcstr[0].c_str());
	m_TimeCodeRecord.t.minutes = (UInt8)atoi(tcstr[1].c_str());
	m_TimeCodeRecord.t.seconds = (UInt8)atoi(tcstr[2].c_str());
	m_TimeCodeRecord.t.frames = (UInt8)atoi(tcstr[3].c_str());
}

/*! ************************************

	\brief Process the "fps" parameter

	Extract the frames per second with "fps 60.0"

***************************************/

void argHandler::parseFPS(void)
{
	try {
		m_fFPS = static_cast<float>(atof(m_Parameters["fps"].c_str()));
	} catch (const exception& e) {
		print(e.what());
		exit(1);
	}

	int i = 0;
	do {
		if (m_fFPS == SUPPORTED_FPS[i]) {
			break;
		}
	} while (++i < (sizeof(SUPPORTED_FPS) / sizeof(SUPPORTED_FPS[0])));

	if (i == (sizeof(SUPPORTED_FPS) / sizeof(SUPPORTED_FPS[0]))) {
		cout << "Unsupported framerate: " << m_fFPS << endl;
		exit(1);
	}

	double dFpsint;
	double dFpsRemainder = modf(m_fFPS, &dFpsint);

	int iDivisor;
	if (dFpsRemainder == 0) {
		iDivisor = 1000;
	} else {
		iDivisor = 1001;
		dFpsint = dFpsint + 1;
	}

	m_TimeCodeDef.flags = 0;
	m_TimeCodeDef.fTimeScale = ((UInt8)dFpsint) * 1000;
	m_TimeCodeDef.frameDuration = iDivisor;
	m_TimeCodeDef.numFrames = (UInt8)dFpsint;
	m_TimeCodeDef.padding = 0;
}

/*! ************************************

	\brief Process the "channels" parameter

	Extract the desired channels with "channels 1,2"

***************************************/

void argHandler::parsechannels(void)
{
	vector<string> channelstr =
		stringtools::split(m_Parameters["channels"], ",");

	if (channelstr.size() != 2) {
		print("2 channels needed for the '-channel 1,2' parameter");
		exit(1);
	}

	m_ChannelRange[0] = atoi(channelstr[0].c_str());
	m_ChannelRange[1] = atoi(channelstr[1].c_str());

	if ((m_ChannelRange[0] < 1) || (m_ChannelRange[1] < 1) ||
		(m_ChannelRange[1] < m_ChannelRange[0])) {
		print(
			"Incorrect input channels. Input channels cannot be less than 1 and the second channel must be larger than the first");
		exit(1);
	}

	m_iNumOfTracks = (m_ChannelRange[1] - m_ChannelRange[0]) + 1;

	if ((m_iNumOfTracks != 2) && (m_iNumOfTracks != 6) &&
		(m_iNumOfTracks != 8) && (m_iNumOfTracks != 24)) {
		print("Total number of tracks is not 2, 6, 8, or 24");
		exit(1);
	}
}

/*! ************************************

	\brief Process all the commands

***************************************/

void argHandler::parsecmds(void)
{
	string cmd;
	unsigned int uCounter = 0;

	for (vector<string>::iterator i = m_CommandLine.begin();
		 i != m_CommandLine.end(); ++i) {
		if ((*i)[0] == '-') {

			// Removes '-'
			cmd = (*i).substr(1, ((*i).size() - 1));

			// Is it a task?
			int j = 0;
			do {
				if (!strcmp(cmd.c_str(), singlecmds[j])) {
					break;
				}
			} while (++j < (sizeof(singlecmds) / sizeof(singlecmds[0])));

			if (j != (sizeof(singlecmds) / sizeof(singlecmds[0]))) {
				// It's a task, accept the command
				m_Tasks.push_back(cmd);

			} else if ((uCounter + 1) < m_CommandLine.size()) {

				// Not followed by another command?
				if (m_CommandLine[uCounter + 1][0] != '-') {
					// Assume what follows is a parameter
					m_Parameters[cmd] = m_CommandLine[uCounter + 1];
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
		++uCounter;
	}
}

/*! ************************************

	\brief Create the argHandler

	Parse the command line and store the results for
	later processing

***************************************/

argHandler::argHandler(int& argc, char* argv[]): m_iNumOfTracks(0), m_fFPS(0.0f)
{
	// Init the tables
	m_ChannelRange[0] = 0;
	m_ChannelRange[1] = 0;

	if (argc < 3) {
		print("Not enough arguments");
		exit(1);
	}

	// Store the command line in this vector
	for (int i = 0; i < argc; ++i) {
		m_CommandLine.push_back(argv[i]);
	}

	// Process the commands
	parsecmds();

	// Was a path found?
	if (vectortools::contains(m_Parameters, string("path"))) {
		parsePath();
	} else {
		print("No input path specified");
		exit(1);
	}

	// If flagaudio is requested, channels is required
	if (vectortools::contains(m_Tasks, string("flagaudio"))) {
		if (vectortools::contains(m_Parameters, string("channels"))) {
			parsechannels();
		} else {
			print("No channel range specified");
			exit(1);
		}
	}

	// Is the setTC (Set TimeCode) command invoked?
	if (vectortools::contains(m_Tasks, string("setTC"))) {
		// It needs an fps parameter
		if (vectortools::contains(m_Parameters, string("fps"))) {
			parseFPS();
		} else {
			print("No FPS specified");
			exit(1);
		}

		// It also needs a tc 0:1:2:3 parameter
		if (vectortools::contains(m_Parameters, string("tc"))) {
			parseTC();
		} else {
			print("No TC specified");
			exit(1);
		}
	}
}
