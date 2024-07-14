/*

File: main.cpp

Abstract: Contains code showing how to create a QuickTime movie from a
		  data file containing just the video frames.

Version: 1.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by
Apple Inc. ("Apple") in consideration of your agreement to the
following terms, and your use, installation, modification or
redistribution of this Apple software constitutes acceptance of these
terms.  If you do not agree with these terms, please do not use,
install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and
subject to these terms, Apple grants you a personal, non-exclusive
license, under Apple's copyrights in this original Apple software (the
"Apple Software"), to use, reproduce, modify and redistribute the Apple
Software, with or without modifications, in source and/or binary forms;
provided that if you redistribute the Apple Software in its entirety and
without modifications, you must retain this notice and the following
text and disclaimers in all such redistributions of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc.
may be used to endorse or promote products derived from the Apple
Software without specific prior written permission from Apple.  Except
as expressly stated in this notice, no other rights or licenses, express
or implied, are granted by Apple herein, including but not limited to
any patent rights that may be infringed by your derivative works or by
other works in which the Apple Software may be incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2007 Apple Inc. All Rights Reserved.

*/

#include <TargetConditionals.h>

#if TARGET_OS_WIN32
#include <QTML.h>
#include <QuickTimeComponents.h>
#include <stdio.h>
#include <stdlib.h>

// Prevent a name collision with the Windows SDK
#define GetProcessInformation Win32GetProcessInformation
#include <windows.h>
#undef GetProcessInformation

#else

// XCode compilers use import for frameworks
#import <QuickTime/QuickTime.h>
#endif

#if defined(_MSC_VER)
#pragma warning(disable : 26429)
#pragma warning(disable : 26472)
#pragma warning(disable : 26481)
#pragma warning(disable : 26490)
#pragma warning(disable : 26814)
#endif
/*

	Constants

*/

static const int kFrameWidth = 200;             // your frameWidth
static const int kFrameHeight = 240;            // your frameHeight
static const TimeScale kMediaTimeScale = 600;   // your media time scale
static const TimeValue kDurationPerSample = 60; // your duration per sample
static const long kNumberOfSamples = 60;        // your number of samples
static const OSType kCodecType = 'raw ';        // your codec type

/*! ************************************

	\brief Creates a data reference from a full path string.

	\param pInputFile Pointer to the full path C-string
	\param ppOutputRef On return this contains the data reference handle
	\param ppOutputType On return this contains the data reference type

***************************************/

static void makeDataRefFromFullPath(
	const char* pInputFile, Handle* ppOutputRef, OSType* ppOutputType)
{
	*ppOutputRef = NULL;

	/* The default encoding for the system;
		untagged 8-bit characters are usually in this encoding */
	const CFStringEncoding sysEncoding = CFStringGetSystemEncoding();
	CFStringRef pDestFileRef =
		CFStringCreateWithCString(kCFAllocatorDefault, pInputFile, sysEncoding);
	if (pDestFileRef) {

		// Create data reference for our raw frames file
		QTNewDataReferenceFromFullPathCFString(pDestFileRef,
			static_cast<QTPathStyle>(kQTNativeDefaultPathStyle), 0, ppOutputRef,
			ppOutputType);
		CFRelease(pDestFileRef);
	}
}

/*! ************************************

	\brief Creates a movie from a file of contiguous video frames

	Once the program has finished, this file will be converted "in place"
	to a QuickTime movie.

	Note: your video frames file must have the first 16 bytes of your
	video frames should be overwritten with the bit pattern that QuickTime
	will later recognize as the start of the 'mdat' atom, which it will
	patch when the movie resource is appended to the file
	(see the "Read Me" file associated with this sample code for more details).
	Then the first compressed video sample can be written at byte offset 16.

	\param pDestMovieFile Your raw video frames file.

***************************************/

static OSErr makeMovieFromVideoFramesFile(const char* pDestMovieFile)
{
	OSErr iError;

	// Special set up for Windows
#if TARGET_OS_WIN32
	InitializeQTML(0);
	iError = GetMoviesError();
	if (iError != noErr) {
		fprintf(stderr, "InitializeQTML() returned %d\n", iError);
		return iError;
	}
#endif

	// Start Quicktime
	EnterMovies();
	iError = GetMoviesError();
	if (iError != noErr) {
		fprintf(stderr, "EnterMovies() returned %d\n", iError);
		return iError;
	}

	Handle hInputFileRef = NULL;
	OSType uInputDataType;
	// create a data reference from the full path to the video frames file
	makeDataRefFromFullPath(pDestMovieFile, &hInputFileRef, &uInputDataType);
	if (!hInputFileRef) {
		fprintf(stderr, "%s", "makeDataRefFromFullPath() failed\n");
		return iError;
	}

	// Create movie
	Movie m = NewMovie(0);
	iError = GetMoviesError();
	if (iError != noErr) {
		fprintf(stderr, "%s", "NewMovie() failed\n");
		return iError;
	}

	// create the video track for the movie
	Track videoT = NewMovieTrack(
		m, Long2Fix(kFrameWidth), Long2Fix(kFrameHeight), kNoVolume);
	iError = GetMoviesError();
	if (iError != noErr) {
		fprintf(stderr, "NewMovieTrack() returned %d\n", iError);
		return iError;
	}

	// create the video track media
	Media videoM = NewTrackMedia(
		videoT, VideoMediaType, kMediaTimeScale, hInputFileRef, uInputDataType);
	iError = GetMoviesError();
	if (iError != noErr) {
		fprintf(stderr, "NewTrackMedia() returned %d\n", iError);
		return iError;
	}

	ImageDescriptionHandle ppImageDescription =
		reinterpret_cast<ImageDescriptionHandle>(
			NewHandleClear(sizeof(ImageDescription)));
	if (ppImageDescription) {

		// create the ImageDescription that will describe our video track media
		// samples
		ImageDescriptionPtr pImageDescription = *ppImageDescription;
		pImageDescription->idSize = sizeof(ImageDescription);
		pImageDescription->cType = kCodecType; // the codec type for your data
		pImageDescription->temporalQuality = codecNormalQuality;
		pImageDescription->spatialQuality = codecNormalQuality;
		pImageDescription->width = static_cast<short>(kFrameWidth);
		pImageDescription->height = static_cast<short>(kFrameHeight);
		pImageDescription->hRes = 72L << 16;
		pImageDescription->vRes = 72L << 16;
		pImageDescription->depth = 32;
		pImageDescription->clutID = -1;

		SampleReference64Record videoRef;
		videoRef.dataOffset.hi = 0;
		videoRef.dataOffset.lo = 0;
		videoRef.dataSize = (kFrameWidth * kFrameHeight * 4) * kNumberOfSamples;
		videoRef.durationPerSample = kDurationPerSample;
		videoRef.numberOfSamples = kNumberOfSamples;
		videoRef.sampleFlags = 0;

		// now add all of our media samples to the movie data file.
		iError = AddMediaSampleReferences64(videoM,
			reinterpret_cast<SampleDescriptionHandle>(ppImageDescription), 1,
			&videoRef, 0);
		if (iError == noErr) {

			const TimeValue mediaDuration =
				kNumberOfSamples * kDurationPerSample;
			// inserts a reference to our media samples into the track.
			// media's rate (1.0 = media's natural playback rate).
			iError = InsertMediaIntoTrack(videoT, 0, 0, mediaDuration, fixed1);
			if (iError == noErr) {

				DataHandler outDataHandler;
				// opens a data handler for our movie storage (the video frames
				// file)

				// create a data reference from the full path to the video
				// frames file
				iError = OpenMovieStorage(hInputFileRef, uInputDataType,
					kDataHCanWrite, &outDataHandler);
				if (iError == noErr) {

					// add a movie to our movie storage container
					iError = AddMovieToStorage(m, outDataHandler);
					if (iError == noErr) {

						iError = CloseMovieStorage(outDataHandler);
						outDataHandler = NULL;
					}
				}
			}
		}
		DisposeHandle(reinterpret_cast<Handle>(ppImageDescription));
	}
	return iError;
}

/*! ************************************

	Parses command line input parameters looking for a specified
	path to a video frames file. Then it calls QuickTime routines
	to create a QuickTime movie from this video frames file.

***************************************/

int main(int argc, const char* const* argv)
{
	const char* pInputFile = NULL;
	int i;

	// look for the "-input" parameter which designates the video frames file
	for (i = 1; i < argc; i++) {
		if (!strncmp("-input", argv[i], 2) && ((i + 1) < argc)) {
			pInputFile = argv[++i];
		}
	}

	OSErr iError;
	if (!pInputFile) {
		fprintf(stderr, "%s",
			"movie_from_ref -input /path/to/inputVideoFramesfile\n");
		iError = paramErr;
	} else {
		// create a QuickTime movie from the video frames file
		iError = makeMovieFromVideoFramesFile(pInputFile);
	}
	return iError;
}
