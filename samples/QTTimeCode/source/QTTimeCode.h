//////////
//
//	File:		QTTimeCode.h
//
//	Contains:	QuickTime timecode media handler sample code.
//
//	Written by:	Tim Monroe
//				based on AddTC.c sample code by QT Engineering and Apple
// Developer Technical Support.
//
//	Copyright:	(c) 1998-2000 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <1>	 	05/18/98	rtm		first file; conversion to personal coding
// style; updated to latest headers
//
//////////

//////////
//
// header files
//
//////////

#include "ComApplication.h"

//////////
//
// constants
//
//////////

#define kTimeCodeTrackSize (20 << 16) // initial height of timecode track

#define kTimeCodeDialogID 200
#define kTimeCodeAlertID 201

#define kItemSrcName 4
#define kItemDisplayTimeCode 5
#define kItemTimeScale 7
#define kItemFrameDur 9
#define kItemNumFrames 11

#define kItemUseTC 12
#define kItemUseCounter 13

#define kItemDropFrame 14
#define kItem24Hour (kItemDropFrame + 1)
#define kItemNegOK (kItem24Hour + 1)
#define kItemIsNeg (kItemNegOK + 2)
#define kItemHours (kItemIsNeg + 1)
#define kItemMinutes (kItemHours + 1)
#define kItemSeconds (kItemMinutes + 1)
#define kItemFrames (kItemSeconds + 1)

#define kItemCounter 24
#define kItemBelowVideo 25
#define kFontPopUpMenuControl 26

#define kFontPopUpResID 1000

#if TARGET_OS_MAC
#define kTextBigSize 20
#define kTextRegSize 12
#endif
#if TARGET_OS_WIN32
#define kTextBigSize 70
#define kTextRegSize 12
#endif

//////////
//
// function prototypes
//
//////////

void QTTC_DeleteTimeCodeTracks(Movie theMovie);
OSErr QTTC_AddTimeCodeToMovie(Movie theMovie, OSType theType);
Boolean QTTC_GetTimeCodeOptions(void);
PASCAL_RTN void QTTC_OptionsUserItemProcedure(
	DialogPtr theDialog, short theItem);
void QTTC_SetDialogTextNumber(
	DialogPtr theDialog, short theItem, long theNumber);
void QTTC_SetDialogTextString(
	DialogPtr theDialog, short theItem, StringPtr theString);
Boolean QTTC_ValidateDialogLong(
	DialogPtr theDialog, short theItem, long* theResult);
ControlHandle QTTC_GetDItemHandle(DialogPtr theDialog, short theItem);
ControlHandle QTTC_GetDItemRect(
	DialogPtr theDialog, short theItem, Rect* theRect);
void QTTC_ShowCurrentTimeCode(Movie theMovie);
void QTTC_ShowTimeCodeSource(Movie theMovie);
void QTTC_ShowStringToUser(StringPtr theString);
void QTTC_ToggleTimeCodeDisplay(MovieController theMC);
MediaHandler QTTC_GetTimeCodeMediaHandler(Movie theMovie);
Boolean QTTC_MovieHasTimeCodeTrack(Movie theMovie);
