/*
	File:		QTLoadLibraryUtils.h

	Copyright:	(c) 2002-2006 by Apple Computer, Inc., all rights reserved.

	Set of utility functions for loading of QuickTime libraries, and finding of
	the QuickTime directories.

*/


#ifndef __QTLOADLIBRARYUTILS__
#define __QTLOADLIBRARYUTILS__

#include <ConditionalMacros.h>

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// QuickTime version of LoadLibrary() which searches QT directories for DLLs.
EXTERN_API_C(HINSTANCE) QTLoadLibrary(const char *szDLL);

// Simple routines to mimic GetSystemDirectory() as much as possible.
EXTERN_API_C(UINT) GetQTSystemDirectoryA(LPSTR lpBuffer, UINT uSize);
EXTERN_API_C(UINT) GetQTExtensionDirectoryA(LPSTR lpBuffer, UINT uSize);
EXTERN_API_C(UINT) GetQTApplicationDirectoryA(LPSTR lpBuffer, UINT uSize);
EXTERN_API_C(UINT) GetQTComponentDirectoryA(LPSTR lpBuffer, UINT uSize);

// be Unicode compatible in the next release of QuickTime
#define GetQTSystemDirectory		GetQTSystemDirectoryA
#define GetQTExtensionDirectory		GetQTExtensionDirectoryA
#define GetQTApplicationDirectory	GetQTApplicationDirectoryA
#define GetQTComponentDirectory		GetQTComponentDirectoryA

#ifdef __cplusplus
}
#endif

#define kQuickTimeKeyName			"Software\\Apple Computer, Inc.\\QuickTime"
#define kQTAppsDirKeyName			"InstallDir"
#define kQTSysDirKeyName			"QTSysDir"
#define kQTExtDirKeyName			"QTExtDir"
#define kQTComponentDirKeyName		"QTComponentsDir"

#endif // !__QTLOADLIBRARYUTILS__
