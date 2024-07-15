//////////
//
//	File:		QTUtilities.h
//
//	Contains:	Some utilities for working with QuickTime movies.
//				All utilities start with the prefix "QTUtils_".
//
//	Written by:	Tim Monroe
//				Based on the DTSQTUtilities package by Apple DTS.
//				This is essentially a subset of that package, revised for
// cross-platform use.
//
//	Copyright:	(c) 1996-1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
// <2>	 	02/03/99	rtm		moved non-QTVR-specific utilities from
// QTVRUtilities to here
//
// <1>	 	09/10/97	rtm		first file
//
//////////

#pragma once

//////////
//
// header files
//
//////////

#ifndef __QTUtilities__
#define __QTUtilities__

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __MOVIESFORMAT__
#include <MoviesFormat.h>
#endif

#ifndef __MEDIAHANDLERS__
#include <MediaHandlers.h>
#endif

#ifndef __NUMBERFORMATTING__
#include <NumberFormatting.h>
#endif

#ifndef __QUICKTIMECOMPONENTS__
#include <QuickTimeComponents.h>
#endif

#ifndef __QUICKTIMEVR__
#include <QuickTimeVR.h>
#endif

#ifndef __QTSMOVIE__
#include <QTSMovie.h>
#endif

#ifndef __ENDIAN__
#include <Endian.h>
#endif

#ifndef __GESTALT__
#include <Gestalt.h>
#endif

#ifndef __FIXMATH__
#include <FixMath.h>
#endif

#ifndef __MACWINDOWS__
#include <MacWindows.h>
#endif

#ifndef __PRINTING__
#include <Printing.h>
#endif

#ifndef __SCRIPT__
#include <Script.h>
#endif

#ifndef __SOUND__
#include <Sound.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef __TEXTUTILS__
#include <TextUtils.h>
#endif

#ifndef __TRAPS__
#include <Traps.h>
#endif

//////////
//
// compiler macros
//
//////////

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

//////////
//
// compiler flags
//
//////////

// is movie content-rating capability implemented?
#define CONTENT_RATING_AVAIL 0

//////////
//
// constants
//
//////////

// constants used for QTUtils_SaveMovie
#define kSavePrompt "Save Movie as:"
#define kSaveMovieFileName "untitled.mov"
#define kSaveResName "Movie Resource"

// constants used for QTUtils_PrintMoviePICT
enum eQTUPICTPrinting { kPrintFrame = 1, kPrintPoster = 2 };

// constants used for QTUtils_GetMovieFileLoopingInfo
enum { kNormalLooping = 0, kPalindromeLooping = 1, kNoLooping = 2 };

// version of QT that first supports QT video effects
#define kQTVideoEffectsMinVers 0x0300

// version of QT that first supports full-screen calls
#define kQTFullScreenMinVers 0x0209

// version of QT that first supports wired sprites
#define kQTWiredSpritesMinVers 0x0300

// constants for GetQuickTimePreference/SetQuickTimePreference settings
#define kConnectionSpeedPrefsType FOUR_CHAR_CODE('cspd')
#define kContentRatingPrefsType FOUR_CHAR_CODE('crat')

enum {
	kQTUtilsSpeakerButton = mcFlagSuppressSpeakerButton,
	kQTUtilsStepButtons = mcFlagSuppressStepButtons,
	kQTUtilsBackButton = mcFlagQTVRSuppressBackBtn,
	kQTUtilsZoomButtons = mcFlagQTVRSuppressZoomBtns,
	kQTUtilsHotSpotButton = mcFlagQTVRSuppressHotSpotBtn,
	kQTUtilsTranslateButton = mcFlagQTVRSuppressTranslateBtn,
	kQTUtilsHelpText = mcFlagQTVRSuppressHelpText,
	kQTUtilsHotSpotNames = mcFlagQTVRSuppressHotSpotNames,
	kQTUtilsCustomButton = mcFlagsUseCustomButton,

	// these are to support existing code that uses our older names
	kQTVRSpeakerButton = mcFlagSuppressSpeakerButton,
	kQTVRBackButton = mcFlagQTVRSuppressBackBtn,
	kQTVRZoomButtons = mcFlagQTVRSuppressZoomBtns,
	kQTVRHotSpotButton = mcFlagQTVRSuppressHotSpotBtn,
	kQTVRTranslateButton = mcFlagQTVRSuppressTranslateBtn,
	kQTVRHelpText = mcFlagQTVRSuppressHelpText,
	kQTVRHotSpotNames = mcFlagQTVRSuppressHotSpotNames,
	kQTVRCustomButton = mcFlagsUseCustomButton
};

// default window position
#ifndef kDefaultWindowX
#define kDefaultWindowX 50
#endif

#ifndef kDefaultWindowY
#define kDefaultWindowY 50
#endif

//////////
//
// data types
//
//////////

#if CONTENT_RATING_AVAIL
// structure of content rating data for GetQuickTimePreference
struct ContentRatingPrefsRecord {
	UInt16 fContentRating;
	UInt16 fContentTypes;
};
typedef struct ContentRatingPrefsRecord ContentRatingPrefsRecord;
#endif

//////////
//
// function prototypes
//
//////////

Boolean QTUtils_TrapAvailable(short theTrapWord);
Boolean QTUtils_IsQuickTimeInstalled(void);
Boolean QTUtils_IsQuickTimeCFMInstalled(void);
long QTUtils_GetQTVersion(void);
Boolean QTUtils_HasQuickTimeVideoEffects(void);
Boolean QTUtils_HasFullScreenSupport(void);
Boolean QTUtils_HasWiredSprites(void);
Boolean QTUtils_IsQTVRMovie(Movie theMovie);
Boolean QTUtils_IsStreamedMovie(Movie theMovie);
Boolean QTUtils_IsAutoPlayMovie(Movie theMovie);
Boolean QTUtils_IsControllerBarVisible(MovieController theMC);
short QTUtils_GetControllerBarHeight(MovieController theMC);
void QTUtils_HideControllerBar(MovieController theMC);
void QTUtils_ShowControllerBar(MovieController theMC);
void QTUtils_ToggleControllerBar(MovieController theMC);
void QTUtils_PutControllerBarOnTop(MovieController theMC);
void QTUtils_HideControllerButton(MovieController theMC, long theButton);
void QTUtils_ShowControllerButton(MovieController theMC, long theButton);
void QTUtils_ToggleControllerButton(MovieController theMC, long theButton);
void QTUtils_ResetControllerButton(MovieController theMC, long theButton);
void QTUtils_ShowCustomButton(MovieController theMC);
Boolean QTUtils_IsControllerButtonVisible(
	MovieController theMC, long theButton);
Boolean QTUtils_IsMediaTypeInMovie(Movie theMovie, OSType theMediaType);
Boolean QTUtils_MovieHasTimeCodeTrack(Movie theMovie);
Boolean QTUtils_MovieHasSoundTrack(Movie theMovie);
MediaHandler QTUtils_GetSoundMediaHandler(Movie theMovie);
OSErr QTUtils_UpdateMovieVolumeSetting(Movie theMovie);
OSErr QTUtils_SelectAllMovie(MovieController theMC);
OSErr QTUtils_SelectNoneMovie(MovieController theMC);
ImageDescriptionHandle QTUtils_MakeSampleDescription(
	long theEffectType, short theWidth, short theHeight);
OSErr QTUtils_GetContentRatingFromMovie(
	Movie theMovie, UInt16* theRating, UInt32* theReasons);
OSErr QTUtils_AddContentRatingToMovie(
	Movie theMovie, UInt16 theRating, UInt32 theReasons);
OSErr QTUtils_AddUserDataTextToMovie(
	Movie theMovie, char* theText, OSType theType);
OSErr QTUtils_AddCopyrightToMovie(Movie theMovie, char* theText);
OSErr QTUtils_AddMovieNameToMovie(Movie theMovie, char* theText);
OSErr QTUtils_AddMovieInfoToMovie(Movie theMovie, char* theText);
OSErr QTUtils_GetMovieFileLoopingInfo(Movie theMovie, long* theLoopInfo);
OSErr QTUtils_SetMovieFileLoopingInfo(Movie theMovie, long theLoopInfo);
OSErr QTUtils_SetLoopingStateFromFile(Movie theMovie, MovieController theMC);
OSErr QTUtils_MakeMovieLoop(Movie theMovie, Boolean isPalindrome);
OSErr QTUtils_GetWindowPositionFromFile(Movie theMovie, Point* thePoint);
char* QTUtils_GetTrackName(Track theTrack);
OSErr QTUtils_SetTrackName(Track theTrack, char* theText);
char* QTUtils_MakeTrackNameByType(Track theTrack);
Boolean QTUtils_IsImageFile(FSSpec* theFSSpec);
Boolean QTUtils_IsMovieFile(FSSpec* theFSSpec);
void QTUtils_ConvertFloatToBigEndian(float* theFloat);
StringPtr QTUtils_ConvertCToPascalString(char* theString);
char* QTUtils_ConvertPascalToCString(StringPtr theString);
OSErr QTUtils_DeleteAllReferencesToTrack(Track theTrack);
TimeValue QTUtils_GetFrameDuration(Track theTrack);
long QTUtils_GetFrameCount(Track theTrack);
void QTUtils_GetMaxWindowDepth(
	CWindowPtr theWindow, short* thePixelType, short* thePixelSize);
void QTUtils_GetMaxScreenDepth(
	Rect* theGlobalRect, short* thePixelType, short* thePixelSize);
long QTUtils_GetUsersConnectionSpeed(void);
OSErr QTUtils_SetUsersConnectionSpeed(long theSpeed);
OSErr QTUtils_GetUsersContentRating(UInt32* theType, UInt16* theRating);
OSType QTUtils_GetControllerType(Movie theMovie);
OSErr QTUtils_SetControllerType(Movie theMovie, OSType theType);
MovieController QTUtils_ChangeControllerType(
	MovieController theMC, OSType theType, long theFlags);

#endif // __QTUtilities__
