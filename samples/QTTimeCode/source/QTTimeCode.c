//////////
//
//	File:		QTTimeCode.c
//
//	Contains:	QuickTime timecode media handler sample code.
//
//	Written by:	Tim Monroe
//				Based on AddTC.c sample code by QT Engineering and Apple
// Developer Technical Support.
//
//	Copyright:	(c) 1998-2000 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	<9>	11/08/00	rtm		removed "\p" escape sequences from
// QTTC_OptionsUserItemProcedure
//
//	<8>	10/15/00	rtm		added QTTC_OptionsUserItemProcedure, so user item
// draws on Windows; added font pop-up support to Windows
//
//	<7>	10/10/00	rtm		added font pop-up menu (currently Mac only)
//
//	<6>	06/21/00	rtm		minor tweaks
//
//	<5>	03/17/00	rtm		made changes to get things running under CarbonLib
//
//	<4> 04/07/99	rtm		modified QTTC_AddTimeCodeToMovie to add big-endian
// time code data to movie (fixes problems on Windows)
//
//	<3>	06/05/98	rtm		fixed track geometry calculations in
// QTTC_AddTimeCodeToMovie; other minor cleanup here and there
//
//	<2>	05/21/98	rtm		added QTTC_ShowStringToUser
//
//	<1>	05/18/98	rtm		first file; conversion to personal coding style;
// updated to latest headers; added ability to toggle timecode display in a
// movie containing a timecode track; added ability to extract timecode values
// and source info; ported to Windows
//
//	This source code shows how to add a timecode track to a movie. It also shows
// how to toggle the visibility of a movie's timecode track and how to
// extract timecode values (and other info) from a timecode track.
//
//////////

//////////
//
// header files
//
//////////

#include "QTTimeCode.h"

//////////
//
// global variables
//
//////////

Str255 gSrcName;
Str255 gFontName;            // name of current font
long gFontIndex = 1;         // index in font pop-up menu of current font
TimeScale gTimeScale = 2997; // typical for NTSC video (29.97 frames/sec)
TimeValue gFrameDur = 100;   // duration of a single frame
long gNumFrames = 30;        // frames per second
Boolean gDisplayTimeCode = false;
Boolean gDisplayBelowVideo = true;
Boolean gDropFrameVal = true;
Boolean g24Hour = true;
Boolean gNegOK = false;
Boolean gIsNeg = false;
Boolean gUseTimeCode = true; // ue timecode (true) or counter (false)?
long gHours = 0;
long gMinutes = 0;
long gSeconds = 0;
long gFrames = 0;
long gCounterVal = 0;

// UPP to our options dialog user item procedure
UserItemUPP gOptionsUserItemProcUPP = NULL;

// file reference number for this application's resource file
extern short gAppResFile;

// UPP to our custom dialog event filter
extern ModalFilterUPP gModalFilterUPP;

//////////
//
// QTTC_DeleteTimeCodeTracks
// Remove all existing timecode tracks from the specified movie.
//
//////////

void QTTC_DeleteTimeCodeTracks(Movie theMovie)
{
	Track myTrack = NULL;

	myTrack = GetMovieIndTrackType(
		theMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	while (myTrack != NULL) {
		DisposeMovieTrack(myTrack);
		myTrack = GetMovieIndTrackType(
			theMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	}
}

//////////
//
// QTTC_AddTimeCodeToMovie
// Add a timecode track to the specified movie.
//
//////////

OSErr QTTC_AddTimeCodeToMovie(Movie theMovie, OSType theType)
{
	Track myTypeTrack = NULL;
	Track myTrack = NULL;
	Media myMedia = NULL;
	MediaHandler myHandler = NULL;
	TimeCodeDef myTCDef;
	TimeCodeRecord myTCRec;
	Str63 myString;
	TimeValue myDuration;
	MatrixRecord myMatrix;
	Fixed myWidth;
	Fixed myHeight;
	Fixed myTCHeight;
	long myFlags = 0L;
	TCTextOptions myTextOptions;
	FontInfo myFontInfo;
	TimeCodeDescriptionHandle myDesc = NULL;
	long** myFrameHandle;
	OSErr myErr = noErr;

	//////////
	//
	// find the target track
	//
	//////////

	// get the (first) track of the specified type; this track determines the
	// width of the new timecode track
	myTypeTrack =
		GetMovieIndTrackType(theMovie, 1, theType, movieTrackMediaType);
	if (myTypeTrack == NULL) {
		myErr = trackNotInMovie;
		goto bail;
	}

	// get the dimensions of the target track
	GetTrackDimensions(myTypeTrack, &myWidth, &myHeight);

	//////////
	//
	// create the timecode track and media
	//
	//////////

	myTrack = NewMovieTrack(theMovie, myWidth, kTimeCodeTrackSize, kNoVolume);
	if (myTrack == NULL)
		goto bail;

	myMedia = NewTrackMedia(
		myTrack, TimeCodeMediaType, GetMovieTimeScale(theMovie), NULL, 0);
	if (myMedia == NULL)
		goto bail;

	myHandler = GetMediaHandler(myMedia);
	if (myHandler == NULL)
		goto bail;

	//////////
	//
	// fill in a timecode definition structure; this becomes part of the
	// timecode description
	//
	//////////

	// set the timecode format information flags
	if (gUseTimeCode) {
		myFlags = 0L;
		if (gDropFrameVal)
			myFlags |= tcDropFrame;
		if (gIsNeg)
			myFlags |= tcNegTimesOK;
		if (g24Hour)
			myFlags |= tc24HourMax;

	} else {
		myFlags = tcCounter;
	}

	myTCDef.flags = myFlags;
	myTCDef.fTimeScale = gTimeScale;
	myTCDef.frameDuration = gFrameDur;
	myTCDef.numFrames = gNumFrames;

	//////////
	//
	// fill in a timecode record
	//
	//////////

	if (gUseTimeCode) {
		myTCRec.t.hours = (UInt8)gHours;
		myTCRec.t.minutes = (UInt8)gMinutes; // negative flag is here
		myTCRec.t.seconds = (UInt8)gSeconds;
		myTCRec.t.frames = (UInt8)gFrames;
		if (gIsNeg)
			myTCRec.t.minutes |= tctNegFlag;
	} else {
		myTCRec.c.counter = gCounterVal;
	}

	//////////
	//
	// figure out the timecode track geometry
	//
	//////////

	// get display options to calculate box height
	TCGetDisplayOptions(myHandler, &myTextOptions);
	GetFNum(gFontName, &myTextOptions.txFont);
	TCSetDisplayOptions(myHandler, &myTextOptions);

	// use the starting time to figure out the dimensions of track
	TCTimeCodeToString(myHandler, &myTCDef, &myTCRec, myString);
	TextFont(myTextOptions.txFont);
	TextFace(myTextOptions.txFace);
	TextSize(myTextOptions.txSize);
	GetFontInfo(&myFontInfo);

	// calculate track width and height based on text
	myTCHeight = FixRatio(myFontInfo.ascent + myFontInfo.descent + 2, 1);
	SetTrackDimensions(myTrack, myWidth, myTCHeight);

	GetTrackMatrix(myTrack, &myMatrix);
	if (gDisplayBelowVideo)
		TranslateMatrix(&myMatrix, 0, myHeight);

	SetTrackMatrix(myTrack, &myMatrix);
	SetTrackEnabled(myTrack, gDisplayTimeCode ? true : false);

	TCSetTimeCodeFlags(
		myHandler, gDisplayTimeCode ? tcdfShowTimeCode : 0, tcdfShowTimeCode);

	//////////
	//
	// edit the track media
	//
	//////////

	myErr = BeginMediaEdits(myMedia);
	if (myErr == noErr) {
		long mySize;
		UserData myUserData;

		//////////
		//
		// create and configure a new timecode description handle
		//
		//////////

		mySize = sizeof(TimeCodeDescription);
		myDesc = (TimeCodeDescriptionHandle)NewHandleClear(mySize);
		if (myDesc == NULL)
			goto bail;

		(**myDesc).descSize = mySize;
		(**myDesc).dataFormat = TimeCodeMediaType;
		(**myDesc).timeCodeDef = myTCDef;

		//////////
		//
		// set the source identification information
		//
		//////////

		// the source identification information for a timecode track is stored
		// in a user data item of type TCSourceRefNameType
		myErr = NewUserData(&myUserData);
		if (myErr == noErr) {
			Handle myNameHandle = NULL;

			myErr = PtrToHand(&gSrcName[1], &myNameHandle, gSrcName[0]);
			if (myErr == noErr) {
				myErr = AddUserDataText(myUserData, myNameHandle,
					TCSourceRefNameType, 1, langEnglish);
				if (myErr == noErr)
					TCSetSourceRef(myHandler, myDesc, myUserData);
			}

			if (myNameHandle != NULL)
				DisposeHandle(myNameHandle);

			DisposeUserData(myUserData);
		}

		//////////
		//
		// add a sample to the timecode track
		//
		// each sample in a timecode track provides timecode information for a
		// span of movie time; here, we add a single sample that spans the
		// entire movie duration
		//
		//////////

		// the sample data contains a frame number that identifies one or more
		// content frames that use the timecode; this value (a long integer)
		// identifies the first frame that uses the timecode
		myFrameHandle = (long**)NewHandle(sizeof(long));
		if (myFrameHandle == NULL)
			goto bail;

		myErr = TCTimeCodeToFrameNumber(
			myHandler, &(**myDesc).timeCodeDef, &myTCRec, *myFrameHandle);

		// the data in the timecode track must be big-endian
		**myFrameHandle = EndianS32_NtoB(**myFrameHandle);

		myDuration = GetMovieDuration(theMovie);
		// since we created the track with the same timescale as the movie,
		// we don't need to convert the duration

		myErr = AddMediaSample(myMedia, (Handle)myFrameHandle, 0,
			GetHandleSize((Handle)myFrameHandle), myDuration,
			(SampleDescriptionHandle)myDesc, 1, 0, 0);
		if (myErr != noErr)
			goto bail;
	}

	myErr = EndMediaEdits(myMedia);
	if (myErr != noErr)
		goto bail;

	myErr = InsertMediaIntoTrack(myTrack, 0, 0, myDuration, fixed1);
	if (myErr != noErr)
		goto bail;

	//////////
	//
	// create a track reference from the target track to the timecode track
	//
	//////////

	myErr = AddTrackReference(myTypeTrack, myTrack, TimeCodeMediaType, NULL);

bail:
	if (myDesc != NULL)
		DisposeHandle((Handle)myDesc);

	if (myFrameHandle != NULL)
		DisposeHandle((Handle)myFrameHandle);

	return (myErr);
}

//////////
//
// QTTC_GetTimeCodeOptions
// Display our options dialog box and get the user's selections.
//
//////////

Boolean QTTC_GetTimeCodeOptions(void)
{
	DialogPtr myDialog = NULL;
	short myItem = kStdCancelItemIndex;
	short myKind;
	ControlHandle myControl = NULL;
	Handle myHandle = NULL;
	MenuHandle myMenu = NULL;
	Rect myRect;
	GrafPtr myPort;

	GetPort(&myPort);

	myDialog = GetNewDialog(kTimeCodeDialogID, NULL, (WindowPtr)-1L);
	if (myDialog == NULL)
		goto bail;

	SetDialogDefaultItem(myDialog, kStdOkItemIndex);
	SetDialogCancelItem(myDialog, kStdCancelItemIndex);

	myControl = QTTC_GetDItemHandle(myDialog, kItemDisplayTimeCode);
	SetControlValue(myControl, gDisplayTimeCode);
	myControl = QTTC_GetDItemHandle(myDialog, kItemBelowVideo);
	SetControlValue(myControl, gDisplayBelowVideo);
	myControl = QTTC_GetDItemHandle(myDialog, kItemDropFrame);
	SetControlValue(myControl, gDropFrameVal);
	myControl = QTTC_GetDItemHandle(myDialog, kItem24Hour);
	SetControlValue(myControl, g24Hour);
	myControl = QTTC_GetDItemHandle(myDialog, kItemNegOK);
	SetControlValue(myControl, gNegOK);

	// set a user-item drawing procedure for the pos/neg box
	GetDialogItem(myDialog, kItemIsNeg, &myKind, &myHandle, &myRect);
	SetDialogItem(
		myDialog, kItemIsNeg, myKind, (Handle)gOptionsUserItemProcUPP, &myRect);

	myControl = QTTC_GetDItemHandle(myDialog, kItemUseTC);
	SetControlValue(myControl, gUseTimeCode ? 1 : 0);
	myControl = QTTC_GetDItemHandle(myDialog, kItemUseCounter);
	SetControlValue(myControl, gUseTimeCode ? 0 : 1);

	QTTC_SetDialogTextNumber(myDialog, kItemTimeScale, gTimeScale);
	QTTC_SetDialogTextNumber(myDialog, kItemFrameDur, gFrameDur);
	QTTC_SetDialogTextNumber(myDialog, kItemNumFrames, gNumFrames);
	QTTC_SetDialogTextNumber(myDialog, kItemHours, gHours);
	QTTC_SetDialogTextNumber(myDialog, kItemMinutes, gMinutes);
	QTTC_SetDialogTextNumber(myDialog, kItemSeconds, gSeconds);
	QTTC_SetDialogTextNumber(myDialog, kItemFrames, gFrames);

	QTTC_SetDialogTextNumber(myDialog, kItemCounter, gCounterVal);

	QTTC_SetDialogTextString(myDialog, kItemSrcName, gSrcName);

	// set the current value of the pop-up font menu
	myControl = QTTC_GetDItemRect(myDialog, kFontPopUpMenuControl, &myRect);
	if (myControl != NULL) {
		myMenu = MacGetMenu(kFontPopUpResID);
		if (myMenu != NULL) {
#if ACCESSOR_CALLS_ARE_FUNCTIONS
			// insert the menu into the menu list
			MacInsertMenu(myMenu, kInsertHierarchicalMenu);
			SetControlPopupMenuHandle(myControl, myMenu);
			SetControlPopupMenuID(myControl, kFontPopUpResID);
#else
			PopupPrivateData myPrivateData;

			// insert the menu into the menu list
			MacInsertMenu(myMenu, kInsertHierarchicalMenu);
			myPrivateData.mHandle = myMenu;
			myPrivateData.mID = kFontPopUpResID;
			(**(PopupPrivateDataHandle)(**myControl).contrlData) =
				myPrivateData;
#endif
			// clean out existing menu
			while (CountMenuItems(myMenu))
				DeleteMenuItem(myMenu, 1);

			// add in the available fonts
			AppendResMenu(myMenu, FOUR_CHAR_CODE('FONT'));
			SetControlMaximum(myControl, CountMenuItems(myMenu));
			SetControlValue(myControl, gFontIndex);
		}
	}

#if TARGET_API_MAC_CARBON
	MacShowWindow(GetDialogWindow(myDialog));
#else
	MacShowWindow(myDialog);
#endif

noGood:
	do {
		ModalDialog(gModalFilterUPP, &myItem);
		switch (myItem) {
		case kItemDisplayTimeCode:
			myControl = QTTC_GetDItemHandle(myDialog, kItemDisplayTimeCode);
			SetControlValue(myControl, !GetControlValue(myControl));
			break;

		case kItemBelowVideo:
			myControl = QTTC_GetDItemHandle(myDialog, kItemBelowVideo);
			SetControlValue(myControl, !GetControlValue(myControl));
			break;

		case kItemDropFrame:
			myControl = QTTC_GetDItemHandle(myDialog, kItemDropFrame);
			SetControlValue(myControl, !GetControlValue(myControl));
			break;

		case kItem24Hour:
			myControl = QTTC_GetDItemHandle(myDialog, kItem24Hour);
			SetControlValue(myControl, !GetControlValue(myControl));
			break;

		case kItemNegOK:
			myControl = QTTC_GetDItemHandle(myDialog, kItemNegOK);
			SetControlValue(myControl, !GetControlValue(myControl));
			break;

		case kItemUseTC:
			myControl = QTTC_GetDItemHandle(myDialog, kItemUseTC);
			SetControlValue(myControl, 1);
			myControl = QTTC_GetDItemHandle(myDialog, kItemUseCounter);
			SetControlValue(myControl, 0);
			break;

		case kItemUseCounter:
			myControl = QTTC_GetDItemHandle(myDialog, kItemUseCounter);
			SetControlValue(myControl, 1);
			myControl = QTTC_GetDItemHandle(myDialog, kItemUseTC);
			SetControlValue(myControl, 0);
			break;

		case kItemIsNeg:
			gIsNeg = !gIsNeg;
			myControl = QTTC_GetDItemRect(myDialog, kItemIsNeg, &myRect);

#if TARGET_API_MAC_CARBON
			InvalWindowRect(GetDialogWindow(myDialog), &myRect);
#else
			InvalRect(&myRect);
			DrawDialog(myDialog); // force a redraw (necessary on Windows)
#endif
			break;

		default:
			break;
		}
	} while ((myItem != kStdOkItemIndex) && (myItem != kStdCancelItemIndex));

	if (myItem == kStdOkItemIndex) {
		myControl = QTTC_GetDItemHandle(myDialog, kItemSrcName);
		GetDialogItemText((Handle)myControl, gSrcName);

		myControl = QTTC_GetDItemHandle(myDialog, kItemDisplayTimeCode);
		gDisplayTimeCode = GetControlValue(myControl);
		myControl = QTTC_GetDItemHandle(myDialog, kItemBelowVideo);
		gDisplayBelowVideo = GetControlValue(myControl);
		myControl = QTTC_GetDItemHandle(myDialog, kItemDropFrame);
		gDropFrameVal = GetControlValue(myControl);
		myControl = QTTC_GetDItemHandle(myDialog, kItem24Hour);
		g24Hour = GetControlValue(myControl);
		myControl = QTTC_GetDItemHandle(myDialog, kItemNegOK);
		gNegOK = GetControlValue(myControl);
		myControl = QTTC_GetDItemHandle(myDialog, kItemUseTC);
		gUseTimeCode = (GetControlValue(myControl) != 0);

		myControl = QTTC_GetDItemHandle(myDialog, kFontPopUpMenuControl);
		if (myControl != NULL) {
			MenuHandle myMenu2 = NULL;

#if ACCESSOR_CALLS_ARE_FUNCTIONS
			myMenu2 = GetControlPopupMenuHandle(myControl);
#else
			myMenu2 =
				(**((PopupPrivateDataHandle)(**myControl).contrlData)).mHandle;
#endif
			gFontIndex = GetControlValue(myControl);
			GetMenuItemText(myMenu2, gFontIndex, gFontName);
		}

		if (!QTTC_ValidateDialogLong(myDialog, kItemTimeScale, &gTimeScale))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemFrameDur, &gFrameDur))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemNumFrames, &gNumFrames))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemHours, &gHours))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemMinutes, &gMinutes))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemSeconds, &gSeconds))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemFrames, &gFrames))
			goto noGood;
		if (!QTTC_ValidateDialogLong(myDialog, kItemCounter, &gCounterVal))
			goto noGood;
	}

bail:
	// remove the pop-up menu from the menu list
	if (myMenu != NULL)
#if ACCESSOR_CALLS_ARE_FUNCTIONS
		MacDeleteMenu(GetMenuID(myMenu));
#else
		MacDeleteMenu((**myMenu).menuID);
#endif

	if (myDialog != NULL)
		DisposeDialog(myDialog);

	MacSetPort(myPort);

	return (myItem == kStdOkItemIndex);
}

//////////
//
// QTTC_OptionsUserItemProcedure
// A user-item procedure to draw a box in the options dialog box.
//
//////////

PASCAL_RTN void QTTC_OptionsUserItemProcedure(
	DialogPtr theDialog, short theItem)
{
	Handle myItemHandle = NULL;
	Rect myRect;
	ControlHandle myControl = NULL;
	StringPtr mySign = NULL;

	if (theItem != kItemIsNeg)
		return;

	MacSetPort(GetWindowFromPort(GetDialogPort(theDialog)));

	myControl = QTTC_GetDItemRect(theDialog, kItemIsNeg, &myRect);
	MoveTo(myRect.left + 2, myRect.top + 17);
	MacFrameRect(&myRect);

	MacInsetRect(&myRect, 1, 1);
	EraseRect(&myRect);
	MacInsetRect(&myRect, -1, -1);

	TextSize(kTextBigSize);

	if (gIsNeg)
		mySign = QTUtils_ConvertCToPascalString("-");
	else
		mySign = QTUtils_ConvertCToPascalString("+");

	DrawString(mySign);

	TextSize(kTextRegSize);
	free(mySign);
}

//////////
//
// QTTC_SetDialogTextNumber
// Set and highlight the text of a dialog item.
//
//////////

void QTTC_SetDialogTextNumber(
	DialogPtr theDialog, short theItem, long theNumber)
{
	Str255 myText;

	NumToString(theNumber, myText);
	SetDialogItemText((Handle)QTTC_GetDItemHandle(theDialog, theItem), myText);
	SelectDialogItemText(theDialog, theItem, 0, 32767);
}

//////////
//
// QTTC_SetDialogTextString
// Set and highlight the text of a dialog item.
//
//////////

void QTTC_SetDialogTextString(
	DialogPtr theDialog, short theItem, StringPtr theString)
{
	SetDialogItemText(
		(Handle)QTTC_GetDItemHandle(theDialog, theItem), theString);
	SelectDialogItemText(theDialog, theItem, 0, 32767);
}

//////////
//
// QTTC_ValidateDialogLong
// Validate a long value.
//
//////////

Boolean QTTC_ValidateDialogLong(
	DialogPtr theDialog, short theItem, long* theResult)
{
	Str255 myText;
	ControlHandle myHandle;
	Boolean IsDigitFound = false;
	short myIndex;

	myHandle = QTTC_GetDItemHandle(theDialog, theItem);
	GetDialogItemText((Handle)myHandle, myText);

	for (myIndex = 1; myIndex < myText[0]; myIndex++) {
		if (myText[myIndex] >= '0' && myText[myIndex] <= '9') {
			IsDigitFound = true;
		} else if (IsDigitFound) {
			myText[0] = myIndex - 1;
			break;
		} else if (myText[myIndex] != ' ') {
			SelectDialogItemText(theDialog, theItem, 0, 32767);
			SysBeep(1);
			return (false);
		}
	}

	StringToNum(myText, theResult);
	return (true);
}

//////////
//
// QTTC_GetDItemHandle
// Get the handle of the specified dialog item.
//
//////////

ControlHandle QTTC_GetDItemHandle(DialogPtr theDialog, short theItem)
{
	short myKind;
	ControlHandle myHandle;
	Rect myRect;

	GetDialogItem(theDialog, theItem, &myKind, (Handle*)&myHandle, &myRect);
	return (myHandle);
}

//////////
//
// QTTC_GetDItemRect
// Get the rectangle surrounding the specified dialog item.
//
//////////

ControlHandle QTTC_GetDItemRect(
	DialogPtr theDialog, short theItem, Rect* theRect)
{
	short myKind;
	ControlHandle myHandle;

	GetDialogItem(theDialog, theItem, &myKind, (Handle*)&myHandle, theRect);
	return (myHandle);
}

//////////
//
// QTTC_ShowCurrentTimeCode
// Show (in an alert box) the timecode value for the current movie time.
//
//////////

void QTTC_ShowCurrentTimeCode(Movie theMovie)
{
	MediaHandler myHandler = NULL;
	HandlerError myErr = noErr;
	TimeCodeDef myTCDef;
	TimeCodeRecord myTCRec;

	myHandler = QTTC_GetTimeCodeMediaHandler(theMovie);
	if (myHandler != NULL) {

		// get the timecode for the current movie time
		myErr = TCGetCurrentTimeCode(myHandler, NULL, &myTCDef, &myTCRec, NULL);
		if (myErr == noErr) {
			Str255 myString;

			myErr = TCTimeCodeToString(myHandler, &myTCDef, &myTCRec, myString);
			if (myErr == noErr)
				QTTC_ShowStringToUser(myString);
		}
	}
}

//////////
//
// QTTC_ShowTimeCodeSource
// Show (in an alert box) the timecode source for the specified movie.
//
//////////

void QTTC_ShowTimeCodeSource(Movie theMovie)
{
	MediaHandler myHandler = NULL;
	HandlerError myErr = noErr;
	UserData myUserData;

	myHandler = QTTC_GetTimeCodeMediaHandler(theMovie);
	if (myHandler != NULL) {

		// get the timecode source for the current movie time
		myErr = TCGetCurrentTimeCode(myHandler, NULL, NULL, NULL, &myUserData);
		if (myErr == noErr) {
			Str255 myString = " [No source name!]";
			Handle myNameHandle = NewHandleClear(0);

			GetUserDataText(
				myUserData, myNameHandle, TCSourceRefNameType, 1, langEnglish);
			if (GetHandleSize(myNameHandle) > 0) {
				BlockMove(
					*myNameHandle, &myString[1], GetHandleSize(myNameHandle));
				myString[0] = GetHandleSize(myNameHandle);
			}

			if (myNameHandle != NULL)
				DisposeHandle(myNameHandle);

			QTTC_ShowStringToUser(myString);

			DisposeUserData(myUserData);
		}
	}
}

//////////
//
// QTTC_ShowStringToUser
// Display a string in an alert box.
//
//////////

void QTTC_ShowStringToUser(StringPtr theString)
{
	short mySavedResFile;

	// get the current resource file and set the application's resource file
	mySavedResFile = CurResFile();
	UseResFile(gAppResFile);

	ParamText(theString, NULL, NULL, NULL);
	Alert(kTimeCodeAlertID, NULL);

	// restore the original resource file
	UseResFile(mySavedResFile);
}

//////////
//
// QTTC_ToggleTimeCodeDisplay
// Toggle the current state of timecode display.
//
//////////

void QTTC_ToggleTimeCodeDisplay(MovieController theMC)
{
	Movie myMovie = MCGetMovie(theMC);
	Track myTrack = NULL;
	MediaHandler myHandler = NULL;
	long myFlags = 0L;

	// get the (first) timecode track in the specified movie
	myTrack = GetMovieIndTrackType(
		myMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	if (myTrack != NULL) {

		// get the timecode track's media handler
		myHandler = QTTC_GetTimeCodeMediaHandler(myMovie);
		if (myHandler != NULL) {

			// toggle the show-timecode flag
			TCGetTimeCodeFlags(myHandler, &myFlags);
			myFlags ^= tcdfShowTimeCode;
			TCSetTimeCodeFlags(myHandler, myFlags, tcdfShowTimeCode);

			// toggle the track enabled state
			SetTrackEnabled(myTrack, !GetTrackEnabled(myTrack));

			// now tell the movie controller the movie has changed,
			// so that the movie rectangle gets updated correctly
			MCMovieChanged(theMC, myMovie);
		}
	}
}

//////////
//
// QTTC_GetTimeCodeMediaHandler
// Get the media handler for the first timecode track in the specified movie.
//
//////////

MediaHandler QTTC_GetTimeCodeMediaHandler(Movie theMovie)
{
	Track myTrack = NULL;
	Media myMedia = NULL;
	MediaHandler myHandler = NULL;

	// get the (first) timecode track in the specified movie
	myTrack = GetMovieIndTrackType(
		theMovie, 1, TimeCodeMediaType, movieTrackMediaType);
	if (myTrack != NULL) {
		// get the timecode track's media and media handler
		myMedia = GetTrackMedia(myTrack);
		if (myMedia != NULL)
			myHandler = GetMediaHandler(myMedia);
	}

	return (myHandler);
}

//////////
//
// QTTC_MovieHasTimeCodeTrack
// Determine whether the specified movie contains a timecode track.
//
//////////

Boolean QTTC_MovieHasTimeCodeTrack(Movie theMovie)
{
	return (GetMovieIndTrackType(
				theMovie, 1, TimeCodeMediaType, movieTrackMediaType) != NULL);
}
