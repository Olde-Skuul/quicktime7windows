#include <QTLoadLibraryUtils.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <io.h>

// Watcom's libraries don't have this and Quicktime
// requires this
#if defined(__WATCOMC__)
int _fltused = 0;
#endif

extern void GetQTFolderFromRegistry(
	const char* pSubKey, const char* pValueName, char* pBuffer, UINT uSize);
extern UINT GetPathToQuickTimeFolder(
	char* pBuffer, UINT uSize, UINT* pReserved);

/*! ************************************

	\brief Ensure a pathname ends with a slash

	The pathname will be in the buffer is tested if it ends
	with a '\\' or a '/'. If not, a '\\' is appended

	\param pBuffer Pointer to a buffer of the string

***************************************/

static void EndWithWindowsSlashes(char* pBuffer)
{
	// Length of the string
	size_t uLen = strlen(pBuffer);

	// Is it already properly terminated?
	if ((pBuffer[uLen - 1] != '\\') && (pBuffer[uLen - 1] != '/')) {

		// Force an ending slash
		pBuffer[uLen] = '\\';
		pBuffer[uLen + 1] = 0;
	}
}
/*! ************************************

	\brief Find a Quicktime folder by reading the registry

	This function is a functional equivalent to the Quicktime for
	Windows internal function GetQTFolderFromRegistry(). It will
	query a registry value from the HKEY_LOCAL_MACHINE root
	and will return the value.

	On success, the pathname will be in the buffer with a
	'\' or a '/' ending the string. Otherwise, the string
	will be empty on failure.

	\note Quicktime for Windows is a 32 bit API. Don't use it
		for 64 bit applications.

	\param pSubKey Pointer to registry sub key
	\param pValueName Pointer to the registry value name to a string
	\param pBuffer Pointer to a buffer to receive the string
	\param uSize Number of bytes in size of the string buffer

***************************************/

void GetQTFolderFromRegistry(
	const char* pSubKey, const char* pValueName, char* pBuffer, UINT uSize)
{
	// Temp size
	DWORD uLength = uSize;

	// Open the initial key
	HKEY pKey = NULL;

	// Init the output
	pBuffer[0] = 0;

	// Open the key
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, pSubKey, 0, KEY_QUERY_VALUE, &pKey) ==
		ERROR_SUCCESS) {

		// Is the key valid
		if (pKey) {

			// Read in the directory name and continue if there was data
			if ((RegQueryValueExA(pKey, pValueName, NULL, NULL, (BYTE*)pBuffer,
					 &uLength) == ERROR_SUCCESS) &&
				pBuffer[0]) {

				// Force a slash to end the string
				EndWithWindowsSlashes(pBuffer);
			}
		}
	}

	// Release the key, if loaded
	if (pKey) {
		RegCloseKey(pKey);
	}
}

/*! ************************************

	\brief Find the Quicktime folder

	This function is a functional equivalent to the Quicktime for
	Windows function GetPathToQuickTimeFolder(). It will
	query a registry values from the HKEY_LOCAL_MACHINE root
	and if found, it will return the path. Otherwise, it will check
	the system folder for the file Quicktime.qts.

	On success, the pathname will be in the buffer with a
	'\' or a '/' ending the string. Otherwise, the string
	will be empty on failure.

	\note Quicktime for Windows is a 32 bit API. Don't use it
	for 64 bit applications.

	\windowsonly

	\param pBuffer Pointer to a buffer to receive the string
	\param uSize Number of bytes in size of the string buffer
	\param pReserved Reserved, pass \ref nullptr

	\return \ref TRUE if successful, \ref FALSE if not

***************************************/

UINT GetPathToQuickTimeFolder(char* pBuffer, UINT uSize, UINT* pReserved)
{
	// Temp buffer
	char Temp[MAX_PATH];

	// Assume failure
	UINT bResult = FALSE;

	// Only execute if there's a valid output buffer
	if (pBuffer) {
		HMODULE hQuickTime;

		// Clear the output buffer
		pBuffer[0] = 0;

		// Set the reserved value if there was a passed pointer
		if (pReserved) {
			pReserved[0] = 0;
		}

		// See if the module is already loaded
		hQuickTime = GetModuleHandleA("QuickTime.qts");
		if (hQuickTime) {

			// Get the path from the loaded module
			if (GetModuleFileNameA(hQuickTime, Temp, sizeof(Temp)) > 0) {

				// Remove the string "Quicktime.qts"
				size_t uStrLength = strlen(Temp) - 13;
				if (((int)uStrLength > 0) && ((uStrLength + 1) > uSize)) {
					// Copy up the string minus the ending "Quicktime.qts"
					// Note: This will end the string with a '\'
					strncpy(pBuffer, Temp, uStrLength);
					pBuffer[uStrLength] = 0;
				}
			}

			// hQuicktime doesn't need to be released.
		}

		// No path yet?
		if (!pBuffer[0]) {

			// Try grabbing from the registry
			GetQTFolderFromRegistry(
				kQuickTimeKeyName, kQTSysDirKeyName, pBuffer, uSize);

			// Detect if the directory exists
			if (pBuffer[0] && _access(pBuffer, 0)) {
				// Failed
				pBuffer[0] = 0;
			}
		}

		// No path yet?
		if (!pBuffer[0]) {

			// Try again, but using another key
			GetQTFolderFromRegistry(
				kQuickTimeKeyName, "QuickTime.qts folder", pBuffer, uSize);

			// Detect if the directory exists
			if (pBuffer[0] && _access(pBuffer, 0)) {
				// Failed
				pBuffer[0] = 0;
			}
		}

		// No path yet?
		if (!pBuffer[0]) {

			// Try the windows directory
			if ((GetSystemDirectoryA(pBuffer, uSize) > 0) ||
				_access(pBuffer, 0)) {
				pBuffer[0] = 0;
			}
		}

		// If something was found, ensure it ends with a slash

		if (pBuffer[0]) {
			EndWithWindowsSlashes(pBuffer);
		}

		// Return TRUE if there was something in the buffer
		bResult = (pBuffer[0] != 0);
	}
	return bResult;
}

/*! ************************************

	\brief Find the Quicktime folder

	This function is a functional equivalent to the Quicktime for
	Windows function GetQTSystemDirectoryA(). It will
	query a registry values from the HKEY_LOCAL_MACHINE root
	and if found, it will return the path. Otherwise, it will check
	the system folder for the file Quicktime.qts.

	On success, the pathname will be in the buffer with a
	'\' or a '/' ending the string. Otherwise, the string
	will be empty on failure.

	\note Quicktime for Windows is a 32 bit API. Don't use it
	for 64 bit applications.

	\param pBuffer Pointer to a buffer to receive the string
	\param uSize Number of bytes in size of the string buffer

	\return Length of the returned string in chars.

***************************************/

UINT GetQTSystemDirectoryA(char* pBuffer, UINT uSize)
{
	GetPathToQuickTimeFolder(pBuffer, uSize, NULL);
	return (UINT)strlen(pBuffer);
}

/*! ************************************

	\brief Find the Quicktime extensions folder

	This function is a functional equivalent to the Quicktime for
	Windows function GetQTExtensionDirectoryA(). It will
	query a registry value from the HKEY_LOCAL_MACHINE root
	and if found, it will return the path.

	On success, the pathname will be in the buffer with a
	'\' or a '/' ending the string. Otherwise, the string
	will be empty on failure.

	\note Quicktime for Windows is a 32 bit API. Don't use it
	for 64 bit applications.

	\param pBuffer Pointer to a buffer to receive the string
	\param uSize Number of bytes in size of the string buffer

	\return Length of the returned string in chars.

***************************************/

UINT GetQTExtensionDirectoryA(char* pBuffer, UINT uSize)
{
	char SystemDirName[256];
	char QTFolderName[256];

	// Try getting it from the registry
	GetQTFolderFromRegistry(
		kQuickTimeKeyName, kQTExtDirKeyName, pBuffer, uSize);
	if (!pBuffer[0]) {

		// Didn't find it?
		// Make space for "Quicktime\"
		uSize -= 10;

		// Read in the system directory name
		GetSystemDirectoryA(SystemDirName, uSize);

		// Find where Quicktime should be
		GetPathToQuickTimeFolder(QTFolderName, uSize, NULL);

		// Did it find it?
		if (QTFolderName[0]) {

			// Make sure the folder ends with a slash
			EndWithWindowsSlashes(SystemDirName);

			// Copy up the Quicktime folder name
			strcpy(pBuffer, QTFolderName);

			// Was the folder found the Windows system folder?
			if (!strcmp(SystemDirName, QTFolderName)) {

				// Since it's the Windows folder that's found, append the
				// string QuickTime\ to the end of the pathname
				// to denote the old location of where Quicktime
				// extensions were stored.
				strcat(pBuffer, "QuickTime\\");
			}
		}
	}
	return (UINT)strlen(pBuffer);
}

/*! ************************************

	\brief Find the Quicktime components folder

	This function is a functional equivalent to the Quicktime for
	Windows function GetQTComponentDirectoryA(). It will
	query a registry value from the HKEY_LOCAL_MACHINE root
	and if found, it will return the path.

	On success, the pathname will be in the buffer with a
	'\' or a '/' ending the string. Otherwise, the string
	will be empty on failure.

	\note Quicktime for Windows is a 32 bit API. Don't use it
	for 64 bit applications.

	\param pBuffer Pointer to a buffer to receive the string
	\param uSize Number of bytes in size of the string buffer

	\return Length of the returned string in chars.

***************************************/

UINT GetQTComponentDirectoryA(char* pBuffer, UINT uSize)
{
	GetQTFolderFromRegistry(
		kQuickTimeKeyName, kQTComponentDirKeyName, pBuffer, uSize);
	if (!pBuffer[0]) {

		GetSystemDirectoryA(pBuffer, uSize);

		EndWithWindowsSlashes(pBuffer);

		// Since it's the Windows folder that's found, append the
		// string QuickTime\ to the end of the pathname
		// to denote the old location of where Quicktime
		// extensions were stored.
		strcat(pBuffer, "QuickTime\\");
	}
	return (UINT)strlen(pBuffer);
}

/*! ************************************

	\brief Find the Quicktime application folder

	This function is a functional equivalent to the Quicktime for
	Windows function GetQTApplicationDirectoryA(). It will
	query a registry value from the HKEY_LOCAL_MACHINE root
	and if found, it will return the path.

	On success, the pathname will be in the buffer with a
	'\' or a '/' ending the string. Otherwise, the string
	will be empty on failure.

	\note Quicktime for Windows is a 32 bit API. Don't use it
	for 64 bit applications.

	\param pBuffer Pointer to a buffer to receive the string
	\param uSize Number of bytes in size of the string buffer

	\return Length of the returned string in chars.

***************************************/

UINT GetQTApplicationDirectoryA(char* pBuffer, UINT uSize)
{
	GetQTFolderFromRegistry(
		kQuickTimeKeyName, kQTAppsDirKeyName, pBuffer, uSize);
	return (UINT)strlen(pBuffer);
}

/*! ************************************

	\brief Locate and load a Quicktime DLL

	This function is a functional equivalent to the Quicktime for
	Windows function QTLoadLibrary().

	\note Do not pass in full pathnames. Only pass in the name of the DLL and
	nothing more. If a full pathname is available, use a direct call to
	LoadLibraryA(const char *) instead.

	\note Quicktime for Windows is a 32 bit API. Don't use it
	for 64 bit applications.

	\param pDLLName Pointer to the DLL requested

	\return HINSTANCE of the DLL or \ref nullptr on failure

***************************************/

HINSTANCE QTLoadLibrary(const char* pDLLName)
{
	char Temp[MAX_PATH];
	char FinalPathname[MAX_PATH];

	// Assume no DLL
	HINSTANCE pResult = NULL;

	// Length of the DLL's name
	const size_t uDLLNameLength = strlen(pDLLName);

	// Get the path for Quicktime
	if (GetPathToQuickTimeFolder(FinalPathname,
			(UINT)(sizeof(FinalPathname) - uDLLNameLength), NULL)) {

		strcpy(Temp, FinalPathname);
		strcat(Temp, pDLLName);

		pResult = LoadLibraryA(Temp);
		if (!pResult) {

			// Clear the error
			GetLastError();
			if (GetQTExtensionDirectoryA(FinalPathname,
					(UINT)(sizeof(FinalPathname) - uDLLNameLength))) {
				strcpy(Temp, FinalPathname);
				strcat(Temp, pDLLName);
				pResult = LoadLibraryA(Temp);
			}
		}
	}
	return pResult;
}
