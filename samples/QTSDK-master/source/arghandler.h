/***************************************

	Command line argument handler

***************************************/

#ifndef __ARGHANDLER_H__
#define __ARGHANDLER_H__

#ifndef __VECTORTOOLS_H__
#include "vectortools.h"
#endif

#include <iostream>

// Quicktime declares NAN if not present, so include
// math here to make sure Quicktime doesn't override NAN
#include <math.h>

#if !defined(__MACH__)
// Quicktime for Windows
#ifndef __QUICKTIMECOMPONENTS__
#include <QuickTimeComponents.h>
#endif

#else
// Quicktime for MacOSX
#ifndef __QUICKTIME__
#include <QuickTime/QuickTime.h>
#endif
#endif

// Simple print function
template<typename T>
void print(T toprint)
{
	std::cout << toprint << std::endl;
}

class argHandler {
private:
	/** Command line parameters */
	std::vector<std::string> m_CommandLine;

	/** Commands placed in a private map */
	commands m_Parameters;

	void parsePath(void);
	void parseTC(void);
	void parsechannels(void);
	void parsecmds(void);
	void parseFPS(void);

public:
	/** List of commands to execute */
	std::vector<std::string> m_Tasks;

	/** Range of channels, set with "channel 0,3" */
	int m_ChannelRange[2];

	/** Number of movie tracks found */
	int m_iNumOfTracks;

	/** Filename path, set with "path" */
	std::string m_PathName;

	/** Quicktime Timecode record */
	TimeCodeRecord m_TimeCodeRecord;

	/** Quicktime Timecode definition */
	TimeCodeDef m_TimeCodeDef;

	/** Desired frames per second, set with "fps" */
	float m_fFPS;

	argHandler(int& argc, char* argv[]);
	inline const commands& getparams(void) const
	{
		return m_Parameters;
	}
};

#endif
