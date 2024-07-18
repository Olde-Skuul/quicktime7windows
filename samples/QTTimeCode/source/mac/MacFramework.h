//////////
//
//
//	File:		MacFramework.h
//
//	Contains:	Basic functions for windows, menus, and similar things.
//
//	Written by:	Tim Monroe
//
//	Copyright:	(c) 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//  <1> 	11/05/99	rtm		first file
//
//////////

#pragma once

//////////
//
// header files
//
//////////

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __CONTROLDEFINITIONS__
#include <ControlDefinitions.h>
#endif

#ifndef __CONTROLS__
#include <Controls.h>
#endif

#ifndef __DEVICES__
#include <Devices.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __DISKINIT__
#include <DiskInit.h>
#endif

#ifndef __FIXMATH__
#include <FixMath.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __MACMEMORY__
#include <MacMemory.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __PROCESSES__
#include <Processes.h>
#endif

#ifndef __QUICKTIMECOMPONENTS__
#include <QuickTimeComponents.h>
#endif

#ifndef __SEGLOAD__
#include <SegLoad.h>
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __TRAPS__
#include <Traps.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#include "ComFramework.h"

//////////
//
// constants
//
//////////

// size of the block of memory used for our grow zone procedure
#define kEmergencyMemorySize 40 * 1024L
#define kExtraStackSpaceSize 32 * 1024L // amount of additional stack space
#define kWNEDefaultSleep 0              // WaitNextEvent sleep time

// number of times to call EventAvail at application startup
#define kBroughtToFront 3

// resource IDs for dialogs
#define kAboutBoxID 128   // dialog ID for About box
#define kAlertErrorID 129 // dialog ID for warning box

// resource ID for string resource containing application's name
#define kAppNameResID 1000
#define kAppNameResIndex 1

#define kDefaultWindowTitle "\puntitled"
#define kDefaultWindowRect {10, 10, 480, 640}

//////////
//
// function prototypes
//
//////////

static void QTFrame_InitMacEnvironment(long theNumMoreMasters);
pascal long QTFrame_GrowZoneProcedure(Size theBytesNeeded);
static Boolean QTFrame_InitMenuBar(void);
static void QTFrame_MainEventLoop(void);
void QTFrame_HandleEvent(EventRecord* theEvent);
void QTFrame_HandleMenuCommand(long theMenuResult);
void QTFrame_HandleKeyPress(EventRecord* theEvent);
PASCAL_RTN void QTFrame_StandardUserItemProcedure(
	DialogPtr theDialog, short theItem);
PASCAL_RTN Boolean QTFrame_StandardModalDialogEventFilter(
	DialogPtr theDialog, EventRecord* theEvent, short* theItemHit);
static Boolean QTFrame_CheckMovieControllers(EventRecord* theEvent);
void QTFrame_ShowWarning(Str255 theMessage, OSErr theErr);