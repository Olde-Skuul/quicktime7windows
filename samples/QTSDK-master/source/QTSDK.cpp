/***************************************

	Quicktime time code processing tool

***************************************/

#include "QTSDK.h"
#include "arghandler.h"
#include "disney24.h"

// Include Quicktime

#if !defined(__MACH__)

// Quicktime for Windows
#ifndef __CoreAudioTypes_h__
#include <CoreAudioTypes.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __QTML__
#include <QTML.h>
#endif

#else

#ifndef __CoreAudio_h__
#include <CoreAudio/CoreAudio.h>
#endif

#ifndef __MOVIES__
#include <QuickTime/Movies.h>
#endif

#ifndef __QTML__
#include <QuickTime/QTML.h>
#endif
#endif

using namespace std;

// List of errors returned for each state of the application
typedef map<string, OSErr> errorDict;

/***************************************

	Initialize Quicktime

***************************************/

static void initQT(errorDict* errors)
{
	errorDict& rVal = *errors;
#if !defined(__MACH__)
	rVal["init"] = InitializeQTML(0L);
#endif
	rVal["enter"] = EnterMovies();
}

/***************************************

	Get the track pointers from the quicktime movie

	Return the pointers as an array of Track pointers

***************************************/

static Track* getTracks(Movie pMovie, const int pRange[2])
{
	const int iNumOfTracks = (pRange[1] - pRange[0]) + 1;

	Track* pAllTracks = new Track[iNumOfTracks];
	for (int i = 0; i < iNumOfTracks; i++) {
		pAllTracks[i] = GetMovieTrack(pMovie, (pRange[0] + i));
	}
	return pAllTracks;
}

/***************************************

	Create the AudioChannelLayouts

***************************************/

static void buildLayouts(
	int iTotalChs, AudioChannelLayout* layouts, bool bMe6ch)
{
	AudioChannelLabel currentch;
	for (int i = 0; i < iTotalChs; i++) {
		if (bMe6ch) {
			if (i % 2 == 0) {
				currentch = 1;
			} else {
				currentch = 2;
			}

			layouts[i].mChannelLayoutTag =
				kAudioChannelLayoutTag_UseChannelDescriptions;
			layouts[i].mChannelBitmap = 0;
			layouts[i].mNumberChannelDescriptions = 1;
			layouts[i].mChannelDescriptions[0].mChannelLabel = currentch;
			layouts[i].mChannelDescriptions[0].mChannelFlags = 0;
			layouts[i].mChannelDescriptions[0].mCoordinates[0] = 0;
			layouts[i].mChannelDescriptions[0].mCoordinates[1] = 0;
			layouts[i].mChannelDescriptions[0].mCoordinates[2] = 0;
		} else {
			if (i > 5) {
				currentch = static_cast<AudioChannelLabel>(i + 32);
			} else {
				currentch = static_cast<AudioChannelLabel>(i + 1);
			}

			layouts[i].mChannelLayoutTag =
				kAudioChannelLayoutTag_UseChannelDescriptions;
			layouts[i].mChannelBitmap = 0;
			layouts[i].mNumberChannelDescriptions = 1;
			layouts[i].mChannelDescriptions[0].mChannelLabel = currentch;
			layouts[i].mChannelDescriptions[0].mChannelFlags = 0;
			layouts[i].mChannelDescriptions[0].mCoordinates[0] = 0;
			layouts[i].mChannelDescriptions[0].mCoordinates[1] = 0;
			layouts[i].mChannelDescriptions[0].mCoordinates[2] = 0;
		}
	}
}

/***************************************

	Create the AudioChannelLayouts for Disney

***************************************/

static void disneyLayout(int iTotalChs, AudioChannelLayout* layouts)
{
	AudioChannelLabel labelID;
	for (UInt32 i = 0; i < (UInt32)iTotalChs; i++) {
		labelID = static_cast<AudioChannelLabel>(
			Disney::Disney24ChConfig(static_cast<int>(i + 1)));
		layouts[i].mChannelLayoutTag =
			kAudioChannelLayoutTag_UseChannelDescriptions;
		layouts[i].mChannelBitmap = 0;
		layouts[i].mNumberChannelDescriptions = 1;
		layouts[i].mChannelDescriptions[0].mChannelLabel = labelID;
		layouts[i].mChannelDescriptions[0].mChannelFlags = 0;
		layouts[i].mChannelDescriptions[0].mCoordinates[0] = 0;
		layouts[i].mChannelDescriptions[0].mCoordinates[1] = 0;
		layouts[i].mChannelDescriptions[0].mCoordinates[2] = 0;
	}
}

/***************************************

	Flag the Quicktime movie

***************************************/

static void FlagQT(Movie pMovie, const int pChannels[2], int iNumberOfTracks,
	bool bMe6ch, bool bDisney24, errorDict& rConvertErrors)
{
	Track* pWorkingTracks = getTracks(pMovie, pChannels);

	AudioChannelLayout* pLayouts = new AudioChannelLayout[iNumberOfTracks];
	if (bDisney24) {
		disneyLayout(iNumberOfTracks, pLayouts);
	} else {
		buildLayouts(iNumberOfTracks, pLayouts, bMe6ch);
	}

	for (int i = 0; i < iNumberOfTracks; i++) {
		rConvertErrors["SetTrack"] = QTSetTrackProperty(pWorkingTracks[i],
			kQTPropertyClass_Audio, kQTAudioPropertyID_ChannelLayout,
			sizeof(pLayouts[i]), &pLayouts[i]);
	}

	// Don't leak memory
	delete[] pLayouts;
	delete[] pWorkingTracks;
}

/***************************************

	Change the TimeCode on a quicktime movie

***************************************/

static void SetTC(const Movie pMovie, errorDict& rConverterrors,
	TimeCodeRecord* pTimeCodeRecord, TimeCodeDef* pTimeCodeDef)
{
	// Get the main video track
	Track ppVideotrack =
		GetMovieIndTrackType(pMovie, 1, VideoMediaType, movieTrackMediaType);

	// Length of the movie, in time
	TimeValue myMovieDur = GetMovieDuration(pMovie);

	// What's the timescale of the movie?
	TimeScale myTS = GetMovieTimeScale(pMovie);

	long mySize = sizeof(TimeCodeDescription);
	TimeCodeDescriptionHandle ppTCDesc =
		(TimeCodeDescriptionHandle)NewHandleClear(mySize);
	(**ppTCDesc).descSize = mySize;
	(**ppTCDesc).dataFormat = TimeCodeMediaType;
	(**ppTCDesc).resvd1 = 0;
	(**ppTCDesc).resvd2 = 0;
	(**ppTCDesc).dataRefIndex = 1;
	(**ppTCDesc).flags = 0;
	(**ppTCDesc).timeCodeDef = *pTimeCodeDef;

	// Remove all the old time code tracks
	Track ppOldTrack =
		GetMovieIndTrackType(pMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	while (ppOldTrack) {
		DisposeMovieTrack(ppOldTrack);
		ppOldTrack = GetMovieIndTrackType(
			pMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	}

	Track ppNewTCTrack = NewMovieTrack(pMovie, 0, 0, 0);
	Media ppNewTCMedia =
		NewTrackMedia(ppNewTCTrack, TimeCodeMediaType, myTS, NULL, 0);

	long** ppFramenum = (long**)NewHandle(sizeof(long));
	Handle ppFrameHandle = (Handle)ppFramenum;
	TCTimeCodeToFrameNumber(GetMediaHandler(ppNewTCMedia), pTimeCodeDef,
		pTimeCodeRecord, *ppFramenum);
	// Data may be little endian, fix on Intel platforms
	// The frames have to be in Big Endian
	**ppFramenum = EndianS32_NtoB(**ppFramenum);

	rConverterrors["BeginEdits"] = BeginMediaEdits(ppNewTCMedia);
	rConverterrors["AddMediaSample"] =
		AddMediaSample(ppNewTCMedia, ppFrameHandle, 0,
			static_cast<unsigned long>(GetHandleSize(ppFrameHandle)),
			myMovieDur, SampleDescriptionHandle(ppTCDesc), 1, 0, 0);
	rConverterrors["EndEdits"] = EndMediaEdits(ppNewTCMedia);

	rConverterrors["InsertMedia"] =
		InsertMediaIntoTrack(ppNewTCTrack, 0, 0, myMovieDur, fixed1);
	rConverterrors["AddTrackReference"] =
		AddTrackReference(ppVideotrack, ppNewTCTrack, TimeCodeMediaType, NULL);
}

/***************************************

	Main entry point

***************************************/

int main(int argc, char* argv[])
{
	// Process the args, can exit() if input is bad
	argHandler args(argc, argv);

	// Start Quicktime
	errorDict initerrors;
	initQT(&initerrors);

	// Was there a startup error?
	for (errorDict::iterator item = initerrors.begin();
		 item != initerrors.end(); item++) {
		if ((*item).second != 0) {
			cout << "Init QT error in " << "'" << (*item).first.c_str() << "'"
				 << endl;
			exit(1);
		}
	}

	Movie myMovie;
	FSSpec myFileSpec;
	const FSSpec* myFSptr = &myFileSpec;
	short myRefNum;
	short myResID;
	StringPtr myStringPtr = (StringPtr) "WorkingFile";
	Boolean bWasChanged = 0;
	errorDict converterrors;

	// Windows and MacOSX handle paths differently
#if !defined(__MACH__)
	converterrors["PathToSpec"] = NativePathNameToFSSpec(
		const_cast<char*>(args.m_PathName.c_str()), (FSSpec*)myFSptr, 0);
#else
	Boolean isdir;
	FSRef fsref;
	converterrors["PathToSpec"] =
		FSPathMakeRef(reinterpret_cast<const UInt8*>(args.m_PathName.c_str()),
			&fsref, &isdir);
	FSGetCatalogInfo(
		&fsref, kFSCatInfoNone, NULL, NULL, (FSSpec*)myFSptr, NULL);
#endif

	converterrors["OpenMovie"] = OpenMovieFile(myFSptr, &myRefNum, 0);
	converterrors["NewMovie"] = NewMovieFromFile(
		&myMovie, myRefNum, &myResID, myStringPtr, 0, &bWasChanged);

	if (converterrors["NewMovie"] != 0) {
		cout << "Error opening QT. 'NewMovie' error code: "
			 << converterrors["NewMovie"] << "\n";
		exit(1);
	}

	// Update the timecode
	if (vectortools::contains(args.m_Tasks, string("setTC"))) {
		SetTC(myMovie, converterrors, &args.m_TimeCodeRecord,
			&args.m_TimeCodeDef);
	}

	// Flag the audio tracks
	if (vectortools::contains(args.m_Tasks, string("flagaudio"))) {
		bool bMe6ch = vectortools::contains(args.m_Tasks, string("me6ch"));
		bool bDisney24 =
			vectortools::contains(args.m_Tasks, string("disney24"));

		FlagQT(myMovie, args.m_ChannelRange, args.m_iNumOfTracks, bMe6ch,
			bDisney24, converterrors);
	}

	// Was the movie data changed?
	if (bWasChanged) {
		converterrors["UpdateResource"] = UpdateMovieResource(
			myMovie, myRefNum, myResID, (ConstStr255Param) "Update");
	}

	converterrors["CloseMovie"] = CloseMovieFile(myRefNum);
	converterrors["General"] = GetMoviesError();

	for (errorDict::iterator item2 = converterrors.begin();
		 item2 != converterrors.end(); item2++) {
		if ((*item2).second != 0) {
			cout << "Error from " << (*item2).first.c_str()
				 << ". Error: " << (*item2).second << endl;
			exit(1);
		}
	}

	// All done
	print("Success");
	return 0;
}
