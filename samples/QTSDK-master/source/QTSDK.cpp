#include "general.h"

#if !defined(__MACH__)
#include <CoreAudioTypes.h>
#include <Movies.h>
#include <QTML.h>
#else
#include <CoreAudio/CoreAudio.h>
#include <QuickTime/Movies.h>
#include <QuickTime/QTML.h>
#endif
using namespace std;

namespace vectortools {
bool contains(const vector<string>& vecx, const string& xitem)
{
	return (std::find(vecx.begin(), vecx.end(), xitem) != vecx.end());
}
}

static void initQT(errorDict* errors)
{
	errorDict& rVal = *errors;
#if !defined(__MACH__)
	rVal["init"] = InitializeQTML(0L);
#endif
	rVal["enter"] = EnterMovies();
}

static Track* getTracks(Movie& movie, const int range[2])
{
	const int numOfTracks = (range[1] - range[0]) + 1;
	Track* allTracks = new Track[numOfTracks];
	for (int i = 0; i < numOfTracks; i++) {
		allTracks[i] = GetMovieTrack(movie, (range[0] + i));
	}
	return allTracks;
}

static void buildLayouts(
	int& totalChs, AudioChannelLayout* layouts, bool& me6ch)
{
	AudioChannelLabel currentch;
	for (int i = 0; i < totalChs; i++) {
		if (me6ch) {
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

static void disneyLayout(int& totalChs, AudioChannelLayout* layouts)
{
	AudioChannelLabel labelID;
	for (UInt32 i = 0; i < (UInt32)totalChs; i++) {
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

static void FlagQT(Movie& myMovie, const int channels[2], int& numberOfTracks,
	bool& me6ch, bool& disney24, errorDict& converterrors)
{
	Track* workingTracks = getTracks(myMovie, channels);

	AudioChannelLayout* layouts = new AudioChannelLayout[numberOfTracks];
	if (disney24) {
		disneyLayout(numberOfTracks, layouts);
	} else {
		buildLayouts(numberOfTracks, layouts, me6ch);
	}

	for (int i = 0; i < numberOfTracks; i++) {
		converterrors["SetTrack"] = QTSetTrackProperty(workingTracks[i],
			kQTPropertyClass_Audio, kQTAudioPropertyID_ChannelLayout,
			sizeof(layouts[i]), &layouts[i]);
	}
}

static void SetTC(const Movie& myMovie, errorDict& converterrors,
	TimeCodeRecord& myTRR, TimeCodeDef& myTCD)
{
	Track videotrack =
		GetMovieIndTrackType(myMovie, 1, VideoMediaType, movieTrackMediaType);
	TimeValue myMovieDur = GetMovieDuration(myMovie);
	TimeScale myTS = GetMovieTimeScale(myMovie);

	long mySize = sizeof(TimeCodeDescription);
	TimeCodeDescriptionHandle myDesc =
		(TimeCodeDescriptionHandle)NewHandleClear(mySize);
	(**myDesc).descSize = mySize;
	(**myDesc).dataFormat = TimeCodeMediaType;
	(**myDesc).resvd1 = 0;
	(**myDesc).resvd2 = 0;
	(**myDesc).dataRefIndex = 1;
	(**myDesc).flags = 0;
	(**myDesc).timeCodeDef = myTCD;

	Track oldTrack = GetMovieIndTrackType(
		myMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	while (oldTrack != NULL) {
		DisposeMovieTrack(oldTrack);
		oldTrack = GetMovieIndTrackType(
			myMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	}

	Track newTCtrack = NewMovieTrack(myMovie, 0, 0, 0);
	Media newTCmedia =
		NewTrackMedia(newTCtrack, TimeCodeMediaType, myTS, NULL, 0);

	long** framenum = (long**)NewHandle(sizeof(long));
	Handle frameH = (Handle)framenum;
	TCTimeCodeToFrameNumber(
		GetMediaHandler(newTCmedia), &myTCD, &myTRR, *framenum);
	**framenum = EndianS32_NtoB(**framenum);

	converterrors["BeginEdits"] = BeginMediaEdits(newTCmedia);
	converterrors["AddMediaSample"] = AddMediaSample(newTCmedia, frameH, 0,
		static_cast<unsigned long>(GetHandleSize(frameH)), myMovieDur,
		SampleDescriptionHandle(myDesc), 1, 0, 0);
	converterrors["EndEdits"] = EndMediaEdits(newTCmedia);

	converterrors["InsertMedia"] =
		InsertMediaIntoTrack(newTCtrack, 0, 0, myMovieDur, fixed1);
	converterrors["AddTrackReference"] =
		AddTrackReference(videotrack, newTCtrack, TimeCodeMediaType, NULL);
}

int main(int argc, char* argv[])
{
	argHandler args(argc, argv);
	errorDict initerrors;
	initQT(&initerrors);

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
	Boolean wasChanged;
	errorDict converterrors;

#if !defined(__MACH__)
	converterrors["PathToSpec"] = NativePathNameToFSSpec(
		const_cast<char*>(args.fileURL.c_str()), (FSSpec*)myFSptr, 0);
#else
	Boolean isdir;
	FSRef fsref;
	converterrors["PathToSpec"] = FSPathMakeRef(
		reinterpret_cast<const UInt8*>(args.fileURL.c_str()), &fsref, &isdir);
	FSGetCatalogInfo(
		&fsref, kFSCatInfoNone, NULL, NULL, (FSSpec*)myFSptr, NULL);
#endif
	converterrors["OpenMovie"] = OpenMovieFile(myFSptr, &myRefNum, 0);
	converterrors["NewMovie"] = NewMovieFromFile(
		&myMovie, myRefNum, &myResID, myStringPtr, 0, &wasChanged);

	if (converterrors["NewMovie"] != 0) {
		cout << "Error opening QT. 'NewMovie' error code: "
			 << converterrors["NewMovie"] << "\n";
		exit(1);
	}

	if (vectortools::contains(args.tasks, string("setTC"))) {
		SetTC(myMovie, converterrors, args.TCR, args.TCD);
	}

	if (vectortools::contains(args.tasks, string("flagaudio"))) {
		bool me6ch = vectortools::contains(args.tasks, string("me6ch"));
		bool disney24 = vectortools::contains(args.tasks, string("disney24"));
		{
			FlagQT(myMovie, args.m_channelRange, args.numOfTracks, me6ch,
				disney24, converterrors);
		}
	}

	if (wasChanged) {
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

	print("Success");
}
