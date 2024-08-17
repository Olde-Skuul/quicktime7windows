//////////
//
//	File:		WinPrefix.h
//
//	Contains:	Prefix file for our Windows projects.
//
//	Written by:	Tim Monroe
//
//	Copyright:	(c) 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <1>	 	11/11/99	rtm		first file
//
//////////

#pragma once

#ifndef __Prefix_File__
#define __Prefix_File__

//////////
//
// header files
//
//////////

#include <ConditionalMacros.h>

//////////
//
// compiler macros
//
//////////

#ifndef PASCAL_RTN
#if defined(__WATCOMC__)
#define PASCAL_RTN __declspec(__cdecl)
#else
#define PASCAL_RTN
#endif
#endif

//////////
//
// compiler pragmas
//
//////////

// if we're being compiled by Microsoft Visual C++, turn off some warnings
#if defined(_MSC_VER) && !defined(__MWERKS__)

// ignore unknown pragmas
#pragma warning(disable : 4068)

// ignore conversion from "long" to "short", possible loss of data
#pragma warning(disable : 4244)

// ignore integral size mismatch in argument: conversion supplied
#pragma warning(disable : 4761)

// ignore 'p': unrecognized character escape sequence
#pragma warning(disable : 4129)

// ignore anachronism used: modifiers on data are ignored
#pragma warning(disable : 4229)
#endif

#endif // __Prefix_File__
