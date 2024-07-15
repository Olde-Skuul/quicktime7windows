//////////
//
//	File:		WinFramework.h
//
//	Contains:	Basic functions for windows, menus, and similar things.
//
//	Written by:	Tim Monroe
//
//	Copyright:	(c) 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <1>	 	11/05/99	rtm		first file
//
//////////

#pragma once

//////////
//
// header files
//
//////////

#ifndef __Prefix_File__
#include "WinPrefix.h"
#endif

#ifndef _WINDOWS_
#define GetProcessInformation Win32GetProcessInformation
#include <windows.h>
#undef GetProcessInformation
#endif

#ifndef __FIXMATH__
#include <FixMath.h>
#endif

#ifndef __QTML__
#include <QTML.h>
#endif

#ifndef __SCRAP__
#include <Scrap.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef __malloc_h__
#include <malloc.h>
#endif

#include "ComFramework.h"
#include "ComResource.h"

#include <shellapi.h>

//////////
//
// constants
//
//////////

#define WM_PUMPMOVIE (WM_USER + 0)
#define WM_OPENDROPPEDFILES (WM_USER + 1)
#define USEEXPLORERSTYLE (LOBYTE(LOWORD(GetVersion())) >= 4)

// an arbitrary value that allows our dialog proc to detect the Open File dialog
// box
#define kOpenDialogCustomData 11

// maximum length of a message in the QTFrame_ShowCautionAlert message box
#define kAlertMessageMaxLength 256

// on Windows, the file path separator is '\\'
#define kWinFilePathSeparator (char)'\\'

// some names for the modifier keys
#define VK_MAC_CONTROLKEY VK_MENU
#define VK_MAC_COMMANDKEY VK_CONTROL
#define VK_MAC_SHIFTKEY VK_SHIFT
#define VK_MAC_CAPSKEY VK_CAPITAL

//////////
//
// compiler macros
//
//////////

// macros for reading Mac-style modifier keys on Windows;
// note that the Mac Option key is defined as the combination of the Ctrl and
// Alt keys on Windows
#define QTFrame_IsControlKeyDown(theKeyState) \
	(theKeyState[VK_MAC_CONTROLKEY] & 0x80 ? 1 : 0)
#define QTFrame_IsCommandKeyDown(theKeyState) \
	(theKeyState[VK_MAC_COMMANDKEY] & 0x80 ? 1 : 0)
#define QTFrame_IsShiftKeyDown(theKeyState) \
	(theKeyState[VK_MAC_SHIFTKEY] & 0x80 ? 1 : 0)
#define QTFrame_IsAlphaLockKeyDown(theKeyState) \
	(theKeyState[VK_MAC_CAPSKEY] & 0x80 ? 1 : 0)
#define QTFrame_IsOptionKeyDown(theKeyState) \
	(QTFrame_IsControlKeyDown(theKeyState)) && \
		(QTFrame_IsCommandKeyDown(theKeyState))

//////////
//
// function prototypes
//
//////////

LRESULT CALLBACK QTFrame_FrameWndProc(
	HWND theWnd, UINT theMessage, UINT wParam, LONG lParam);
LRESULT CALLBACK QTFrame_MovieWndProc(
	HWND theWnd, UINT theMessage, UINT wParam, LONG lParam);
void QTFrame_OpenCommandLineMovies(LPSTR theCmdLine);
int QTFrame_ShowCautionAlert(HWND theWnd, UINT theID, UINT theIconStyle,
	UINT theButtonStyle, LPSTR theTitle, LPSTR theArgument);
static UINT APIENTRY QTFrame_DialogProcedure(
	HWND theDialog, UINT theMessage, WPARAM wParam, LPARAM lParam);
static void QTFrame_CalcWindowMinMaxInfo(HWND theWnd, LPMINMAXINFO lpMinMax);
static long QTFrame_GetKeyboardModifiers(void);
