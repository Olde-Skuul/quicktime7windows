//////////
//
//	File:		ComApplication.h
//
//	Contains:	Functions that could be overridden in a specific application.
//
//	Written by:	Tim Monroe
//
//	Copyright:	(c) 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	<1>		11/05/99	rtm		first file; based on earlier sample code
//
//////////

#pragma once

//////////
//
// header files
//
//////////

#ifndef __QUICKTIMEVR__
#include <QuickTimeVR.h>
#endif

#ifndef __TEXTUTILS__
#include <TextUtils.h>
#endif

#ifndef __SCRIPT__
#include <Script.h>
#endif

#if TARGET_OS_MAC
#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif
#include "MacFramework.h"
#endif

#if TARGET_OS_WIN32
#include "WinFramework.h"
#endif

#ifndef __QTUtilities__
#include "QTUtilities.h"
#endif

#include "ComResource.h"

//////////
//
// constants
//
//////////

//////////
//
// structures
//
//////////

// application-specific data
typedef struct ApplicationDataRecord {
	Boolean fBogusField;

} ApplicationDataRecord, *ApplicationDataPtr, **ApplicationDataHdl;

//////////
//
// function prototypes
//
//////////

#if TARGET_OS_MAC
void QTApp_InstallAppleEventHandlers(void);
PASCAL_RTN OSErr QTApp_HandleOpenApplicationAppleEvent(
	const AppleEvent* theMessage, AppleEvent* theReply, long theRefcon);
PASCAL_RTN OSErr QTApp_HandleOpenDocumentAppleEvent(
	const AppleEvent* theMessage, AppleEvent* theReply, long theRefcon);
PASCAL_RTN OSErr QTApp_HandlePrintDocumentAppleEvent(
	const AppleEvent* theMessage, AppleEvent* theReply, long theRefcon);
PASCAL_RTN OSErr QTApp_HandleQuitApplicationAppleEvent(
	const AppleEvent* theMessage, AppleEvent* theReply, long theRefcon);
#endif // TARGET_OS_MAC

// the other function prototypes are in the file MacFramework.h or
// WinFramework.h