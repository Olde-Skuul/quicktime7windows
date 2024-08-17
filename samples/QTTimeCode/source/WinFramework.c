//////////
//
//	File:		WinFramework.c
//
//	Contains:	Code for the QuickTime sample code framework that is specific to
//				Windows.
//
// This code handles windows, menus, events, and other low-level things. Put
// your application-specific code into the file ComApplication.c.
//
// Written by:	Tim Monroe
//
// Based on the QTShell code written by Tim Monroe, which in turn was based on
// the MDIPlayer code written by Brian S. Friedkin (Aug 5, 1996). This current
// version is now very far removed from MDIPlayer.
//
// Copyright: (c) 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
// <11> 07/31/00	rtm		reworked QTFrame_CalcWindowMinMaxInfo to use
// subsystem 4.0 metrics
//
// <10> 07/07/00	rtm		removed QTFrame_CreateMacEditMenu; now we use
// MCGetMenuString
//
// <9>	07/06/00	rtm		made changes to support new parameter to
// QTFrame_AdjustMenus; added the calls QTFrame_CreateMacEditMenu and
// QTFrame_GetKeyboardModifiers
//
// <8> 06/26/00		rtm		added SetFocus call to QTFrame_CreateMovieWindow
//
// <7> 04/03/00		rtm		reworked QTFrame_CalcWindowMinMaxInfo; disabled
// movie window maximize button in WM_CREATE message processing (implementing
// this is left as an exercise...)
//
// <6> 01/14/00 	rtm		reworked window-drawing code (WM_PAINT message) to
// support graphics files
//
// <5> 01/05/00		rtm		minor tweaks to QuickTime initialization in WinMain
//
// <4> 12/26/99		rtm		added WM_LBUTTONDOWN processing to
// QTFrame_MovieWndProc; minor reorganization of QTFrame_MovieWndProc so that
// myMacEvent and myIsHandled are in scope for that message
//
// <3> 12/16/99		rtm		made minor change to QTFrame_MovieWndProc so that
// QTApp_HandleEvent is called even if the window has no movie controller
//
// <2> 11/29/99		rtm		modified "Save changes" dialog box to use Macintosh
// wordings prompted by move to Navigation Services
//
// <1> 11/05/99		rtm		first file; based on earlier sample code
//
//////////

//////////
//
// header files
//
//////////

#include "WinFramework.h"

#include <commdlg.h>

#if defined(_MSC_VER)
// Disable unreferenced formal parameters
#pragma warning(disable : 4100)
#endif

//////////
//
// global variables
//
//////////

BOOL gShuttingDown = false;        // are we shutting down?
BOOL gWeAreSizingWindow = false;   // are we resizing a window?
BOOL gWeAreCreatingWindow = false; // are we creating a window?

HANDLE ghInst;       // the instance of this application
HWND ghWnd;          // the MDI frame window; this window has the menu bar
HWND ghWndMDIClient; // the MDI client window

char gChildName[] = "QTShellChild";
char gMovieType[] = "QuickTime Movie";

short gAppResFile = kInvalidFileRefNum; // file reference number for this
										// application's resource file
FSSpec gAppFSSpec; // file specification for the application itself
char gAppName[20]; // the name of this application

LPSTR gCmdLine; // the command line passed to WinMain

extern Rect gMCResizeBounds; // maximum size for any movie window

ModalFilterUPP gModalFilterUPP = NULL; // UPP to our custom dialog event filter
bool bFirstTime = true;

//////////
//
// WinMain
// The main function for this application.
//
// Set up the application's execution environment; make sure QuickTime (etc.) is
// installed, then start handling events. If we terminate before reaching the
// message loop, we should return 0.
//
//////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR theCmdLine, int nCmdShow)
{
#pragma unused(hPrevInstance)
	HANDLE myAccel;
	HWND myWindowFrame;
	MSG myMsg;
	WNDCLASSEX myWC;
	char myFileName[MAX_PATH];
	DWORD myLength;
	OSErr myErr = noErr;

	ghInst = hInstance;
	gCmdLine = theCmdLine;

	LoadString(hInstance, IDS_APPNAME, gAppName, sizeof(gAppName));

	// register the frame window class
	myWC.cbSize = sizeof(WNDCLASSEX);
	myWC.style = CS_HREDRAW | CS_VREDRAW;
	myWC.lpfnWndProc = QTFrame_FrameWndProc;
	myWC.cbClsExtra = 0;
	myWC.cbWndExtra = 0;
	myWC.hInstance = hInstance;
	myWC.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
	myWC.hCursor = LoadCursor(NULL, IDC_ARROW);
	myWC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	myWC.lpszMenuName = gAppName;
	myWC.lpszClassName = gAppName;
	myWC.hIconSm = LoadImage(
		hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0);

	if (!RegisterClassEx(&myWC)) {
		if (!RegisterClass((LPWNDCLASS)&myWC.style)) {
			return 0;
		}
	}

	// register the movie child window class
	myWC.cbSize = sizeof(WNDCLASSEX);
	myWC.style = 0;
	myWC.lpfnWndProc = QTFrame_MovieWndProc;
	myWC.cbClsExtra = 0;
	myWC.cbWndExtra = 0;
	myWC.hInstance = hInstance;
	myWC.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHILDICON));
	// to avoid having QuickTime VR "fight" with the system over the cursor,
	// we set the client area cursor to NULL; this means that for QuickTime
	// movies, we'll need to change the cursor to an arrow manually; see the
	// handling of the WM_MOUSEMOVE message in QTFrame_MovieWndProc
	myWC.hCursor = NULL;
	myWC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	myWC.lpszMenuName = NULL;
	myWC.lpszClassName = gChildName;
	myWC.hIconSm = LoadImage(
		hInstance, MAKEINTRESOURCE(IDI_CHILDICON), IMAGE_ICON, 16, 16, 0);

	if (!RegisterClassEx(&myWC)) {
		if (!RegisterClass((LPWNDCLASS)&myWC.style)) {
			return 0;
		}
	}

	// load accelerators
	myAccel = LoadAccelerators(hInstance, gAppName);

	// initialize QuickTime Media Layer and QuickTime; alert the user and return
	// 0 if unsuccessful
	myErr = InitializeQTML(0L);
	if (myErr != noErr) {
		MessageBox(NULL,
			"QuickTime is not installed on this computer. Exiting.", gAppName,
			MB_OK | MB_APPLMODAL);
		return 0;
	}

	myErr = EnterMovies();
	if (myErr != noErr) {
		MessageBox(NULL, "Could not initialize QuickTime. Exiting.", gAppName,
			MB_OK | MB_APPLMODAL);
		return 0;
	}

	// get the application's resource file, if it exists
	myLength = GetModuleFileName(
		NULL, myFileName, MAX_PATH); // NULL means: the current process
	if (myLength != 0) {
		NativePathNameToFSSpec(myFileName, &gAppFSSpec, kFullNativePath);

		gAppResFile = FSpOpenResFile(&gAppFSSpec, fsRdWrPerm);
		if (gAppResFile != kInvalidFileRefNum) {
			UseResFile(gAppResFile);
		}
	}

	// do any application-specific initialization that must occur before the
	// frame window is created
	QTApp_Init(kInitAppPhase_BeforeCreateFrameWindow);

	// create the main frame window
	myWindowFrame = CreateWindowA(gAppName, gAppName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ghWnd = myWindowFrame;

	// make sure we got a frame window
	if (myWindowFrame == NULL) {
		return 0;
	}

	// show the window
	ShowWindow(myWindowFrame, nCmdShow);
	UpdateWindow(myWindowFrame);

	// do any application-specific initialization that must occur after the
	// frame window is created
	QTApp_Init(kInitAppPhase_AfterCreateFrameWindow);

	// get and process events until the user quits
	while (GetMessage(&myMsg, NULL, 0, 0)) {
		if (!TranslateMDISysAccel(ghWndMDIClient, &myMsg)) {
			if (!TranslateAccelerator(myWindowFrame, myAccel, &myMsg)) {
				TranslateMessage(&myMsg);
				DispatchMessage(&myMsg);
			}
		}
	}

	// close the application's resource file, if it was previously opened
	if (gAppResFile != kInvalidFileRefNum) {
		CloseResFile(gAppResFile);
	}

	// terminate the QuickTime Media Layer
	ExitMovies();
	TerminateQTML();

	// returns the value from PostQuitMessage
	return (int)myMsg.wParam;
}

//////////
//
// QTFrame_FrameWndProc
// The window procedure for the MDI frame window.
//
//////////

LRESULT CALLBACK QTFrame_FrameWndProc(
	HWND theWnd, UINT theMessage, UINT wParam, LONG lParam)
{
	HWND myChild;

	switch (theMessage) {

	case WM_CREATE: {
		CLIENTCREATESTRUCT myClientStruct = {0};

		myClientStruct.hWindowMenu = GetSubMenu(GetMenu(theWnd), WINDOWMENU);
		myClientStruct.idFirstChild = IDM_WINDOWCHILD;

		// create the MDI client filling the client area
		ghWndMDIClient = CreateWindowA("mdiclient", NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0,
			theWnd, (HMENU)0xCAC, ghInst, (LPVOID)&myClientStruct);

		// set initial menu state
		QTFrame_AdjustMenus(NULL, GetMenu(theWnd), 0L);

		if (ghWndMDIClient != NULL)
			ShowWindow(ghWndMDIClient, SW_SHOW);

		return (0);
	}

	case WM_ACTIVATE:
		// the MDI frame window is being activated or deactivated;
		// activate or deactivate any active child window by sending this
		// message to DefMDIChildProcA
		myChild = (HWND)SendMessage(ghWndMDIClient, WM_MDIGETACTIVE, 0, 0L);
		if (IsWindow(myChild))
			SendMessage(myChild, WM_ACTIVATE, wParam, lParam);
		break;

	case WM_COMMAND: {

		switch (LOWORD(wParam)) {
		case IDM_FILENEW:
		case IDM_FILEOPEN:
		case IDM_FILECLOSE:
		case IDM_EXIT:
			QTFrame_HandleFileMenuItem(NULL, LOWORD(wParam));
			return 0;

		case IDM_FILESAVE:
		case IDM_FILESAVEAS:
			// save the active child window
			myChild = (HWND)SendMessage(ghWndMDIClient, WM_MDIGETACTIVE, 0, 0L);
			if (IsWindow(myChild))
				SendMessage(myChild, WM_COMMAND, wParam, lParam);
			return (0);

		case IDM_WINDOWTILE:
			SendMessage(ghWndMDIClient, WM_MDITILE, 0, 0L);
			return (0);

		case IDM_WINDOWCASCADE:
			SendMessage(ghWndMDIClient, WM_MDICASCADE, 0, 0L);
			return (0);

		case IDM_WINDOWICONS:
			SendMessage(ghWndMDIClient, WM_MDIICONARRANGE, 0, 0L);
			return (0);

		case IDM_WINDOWCLOSEALL: {
			WindowReference myWindow, myNextWindow;

			// walk the window list and destroy any open windows
			myWindow = QTFrame_GetFrontMovieWindow();
			while (myWindow != NULL) {
				myNextWindow = QTFrame_GetNextMovieWindow(myWindow);
				SendMessage(myWindow, WM_CLOSE, 0L, 0L);
				myWindow = myNextWindow;
			}

			return (0);
		}

		case IDM_ABOUT:
			QTFrame_ShowAboutBox();
			return (0);

		default:
			// pass this message to the active child window...
			myChild = (HWND)SendMessage(ghWndMDIClient, WM_MDIGETACTIVE, 0, 0L);
			if (IsWindow(myChild))
				SendMessage(myChild, WM_COMMAND, wParam, lParam);

			// ...then do any application-specific menu handling, if no movie
			// windows are open...
			if (myChild == NULL)
				QTApp_HandleMenu((UInt16)LOWORD(wParam));

			// ...and then pass it to DefFrameProc
			break;
		}
		break;
	}

	case WM_OPENDROPPEDFILES:
		// open any movie files that were dropped onto the application icon
		QTFrame_OpenCommandLineMovies(gCmdLine);
		return (0);

	case WM_INITMENU: {
		long myModifiers = 0L;

		myModifiers = QTFrame_GetKeyboardModifiers();
		if (GetMenu(theWnd) == (HMENU)wParam)
			return (QTFrame_AdjustMenus(
				(HWND)SendMessage(ghWndMDIClient, WM_MDIGETACTIVE, 0, 0L),
				(HMENU)wParam, myModifiers));
		return (1);
	}

	case WM_CLOSE:
		// if we're not already in the process of shutting down,
		// simulate the selection of the Quit menu command
		if (!gShuttingDown) {
			SendMessage(ghWnd, WM_COMMAND, IDM_EXIT, 0L);
			return (0);
		}
		break;

	case WM_DESTROY:
		// do any application-specific shutdown
		QTApp_Stop(kStopAppPhase_AfterDestroyWindows);
		PostQuitMessage(0);
		break;
	}

	return (DefFrameProc(theWnd, ghWndMDIClient, theMessage, wParam, lParam));
}

//////////
//
// QTFrame_MovieWndProc
// The window procedure for a movie window.
//
//////////

LRESULT CALLBACK QTFrame_MovieWndProc(
	HWND theWnd, UINT theMessage, UINT wParam, LONG lParam)
{
	WPARAM myWidth, myHeight;
	MovieController myMC = NULL;
	Movie myMovie = NULL;
	WindowObject myWindowObject = NULL;
	MSG myMsg = {0};
	EventRecord myMacEvent;
	Boolean myIsHandled = false;

	// get the window object, movie, and movie controller for this window
	myWindowObject = QTFrame_GetWindowObjectFromWindow(theWnd);
	if (myWindowObject != NULL) {
		myMC = (**myWindowObject).fController;
		myMovie = (**myWindowObject).fMovie;
	}

	// give the movie controller this message first
	if ((!gShuttingDown) && (theMessage != WM_COMMAND)) {
		LONG myPoints = (LONG)GetMessagePos();

		myMsg.hwnd = theWnd;
		myMsg.message = theMessage;
		myMsg.wParam = wParam;
		myMsg.lParam = lParam;
		myMsg.time = (DWORD)GetMessageTime();
		myMsg.pt.x = LOWORD(myPoints);
		myMsg.pt.y = HIWORD(myPoints);

		// translate a Windows event to a Mac event
		WinEventToMacEvent(&myMsg, &myMacEvent);

		// let the application-specific code have a chance to intercept the
		// event
		myIsHandled = QTApp_HandleEvent(&myMacEvent);

		// pass the Mac event to the movie controller, but only if the movie
		// window isn't minimized
		if (!myIsHandled)
			if (myMC != NULL)
				if (!IsIconic(theWnd))
					myIsHandled = (Boolean)MCIsPlayerEvent(
						myMC, (EventRecord*)&myMacEvent);
	}

	switch (theMessage) {
	case WM_CREATE: {
		LONG myStyles;

		// create a new window object associated with the new window
		QTFrame_CreateWindowObject(theWnd);

		// disable the maximize button
		myStyles = GetWindowLong(theWnd, GWL_STYLE);
		myStyles &= ~WS_MAXIMIZEBOX;
		SetWindowLong(theWnd, GWL_STYLE, myStyles);
	} break;

	case WM_WINDOWPOSCHANGING:
		// don't show the window until we have created a movie and
		// can therefore properly size the window to contain the movie
		if (gWeAreCreatingWindow) {
			WINDOWPOS* lpWindowPos = (WINDOWPOS*)lParam;

			lpWindowPos->flags &= ~SWP_SHOWWINDOW;
		}
		break;

	case WM_WINDOWPOSCHANGED:
		// if a movie window has become minimized, stop the movie
		if (IsIconic(theWnd))
			StopMovie(myMovie);
		break;

	case WM_SIZE:
		// resize the movie and controller to fit the window
		myWidth = LOWORD(lParam);
		myHeight = HIWORD(lParam);

		// we do NOT want to resize the movie controller if the window is
		// minimized, if there is no movie controller, or if we are in the
		// middle of resizing the window
		if (!gWeAreSizingWindow && (myMC != NULL) &&
			(wParam != SIZE_MINIMIZED)) {
			Rect myRect;

			myRect.top = 0;
			myRect.left = 0;
			myRect.right = (short)myWidth;
			myRect.bottom = (short)myHeight;

			MCSetControllerBoundsRect(myMC, &myRect);
		}
		break;

	case WM_MOUSEMOVE:
		// for QuickTime movies (but NOT for QuickTime VR movies), set the
		// cursor to the arrow cursor
		if (myWindowObject != NULL)
			if (!(**myWindowObject).fIsQTVRMovie)
				SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;

	case WM_PUMPMOVIE:
		// we receive this message only to task the movie
		break;

	case WM_LBUTTONDOWN:
		// do any application-specific mouse-button handling, but only if the
		// message hasn't already been handled
		if (!myIsHandled)
			QTApp_HandleContentClick(theWnd, &myMacEvent);
		break;

	case WM_CHAR:
		// do any application-specific key press handling
		QTApp_HandleKeyPress((char)wParam);
		break;

	case WM_PAINT: {
		// do any application-specific drawing in the window
		PAINTSTRUCT myPaintStruct;

		BeginPaint(theWnd, &myPaintStruct);

		// if the window contains an image, draw it using GraphicsImportDraw
		if (myWindowObject != NULL)
			if ((**myWindowObject).fGraphicsImporter != NULL)
				GraphicsImportDraw((**myWindowObject).fGraphicsImporter);

		QTApp_Draw(theWnd);

		EndPaint(theWnd, &myPaintStruct);
	} break;

	case WM_MDIACTIVATE:
		// activate or deactivate the movie controller in the specified window
		QTFrame_ActivateController(theWnd, (HWND)theWnd == (HWND)lParam);
		break;

	case WM_COMMAND: {

		switch (LOWORD(wParam)) {

		case IDM_FILESAVE:
		case IDM_FILESAVEAS:
			QTFrame_HandleFileMenuItem(theWnd, LOWORD(wParam));
			break;

		case IDM_EDITUNDO:
		case IDM_EDITCUT:
		case IDM_EDITCOPY:
		case IDM_EDITPASTE:
		case IDM_EDITCLEAR:
		case IDM_EDITSELECTALL:
		case IDM_EDITSELECTNONE:
			QTFrame_HandleEditMenuItem(theWnd, LOWORD(wParam));
			break;

		default:
			// do any application-specific menu handling
			QTApp_HandleMenu((UInt16)LOWORD(wParam));
			break;
		}

		break;
	} // case WM_COMMAND

	case WM_GETMINMAXINFO:
		QTFrame_CalcWindowMinMaxInfo(theWnd, (LPMINMAXINFO)lParam);
		return (0);

	case WM_CLOSE:
		// prepare to close the window, making sure that any changed data is
		// saved or explicitly discarded; we can still cancel the window closing
		// here
		if (myWindowObject != NULL) {

			// if the window's data is "dirty", give the user a chance to save
			// it
			if ((**myWindowObject).fIsDirty) {
				int myItem;
				char myText[256];
				UINT myAction;

				// get the title of the window
				GetWindowText(theWnd, myText, sizeof(myText));

				// specify the action
				myAction =
					(UINT)(gShuttingDown ? IDS_SAVEONQUIT : IDS_SAVEONCLOSE);

				// display the "Save changes" dialog box
				myItem = QTFrame_ShowCautionAlert(theWnd, myAction,
					MB_ICONEXCLAMATION, MB_YESNOCANCEL, gAppName, myText);
				switch (myItem) {
				case kSaveChanges:
					// save the data in the window
					QTFrame_UpdateMovieFile(theWnd);
					break;

				case kCancelClose:
					// do not close the window and do not quit the application
					gShuttingDown = false;
					return (0);

				case kDontSaveChanges:
					// discard any unsaved changes (that is, don't do anything)
					break;

				default:
					// unexpected item selected; just return
					return (0);
				}
			}
		} // if (myWindowObject != NULL)

		// if we got to this point, it's okay to close and destroy the window
		SendMessage(ghWndMDIClient, WM_MDIDESTROY, (WPARAM)theWnd, 0L);
		break;

	case WM_DESTROY:
		// when we get this message,
		// the window has been removed from the screen and its associated data
		// must be destroyed
		if (myWindowObject != NULL)
			QTFrame_CloseWindowObject(myWindowObject);

		SetWindowLong(theWnd, GWL_USERDATA, 0);

		// destroy the port association
		DestroyPortAssociation((CGrafPtr)GetHWNDPort(theWnd));

		break;
	}

	return (DefMDIChildProcA(theWnd, theMessage, wParam, lParam));
}

//////////
//
// QTFrame_QuitFramework
// Do any framework-specific shut-down.
//
//////////

void QTFrame_QuitFramework(void)
{
	// set our global flag to indicate we're shutting down
	gShuttingDown = true;

	// do application-specific processing that must occur before movie windows
	// are closed
	QTApp_Stop(kStopAppPhase_BeforeDestroyWindows);

	// close all open movie windows; note that the user can cancel the shutting
	// down
	QTFrame_CloseMovieWindows();

	// close the frame window, if we're still shutting down
	if (gShuttingDown)
		SendMessage(ghWnd, WM_CLOSE, 0, 0L);
}

//////////
//
// QTFrame_OpenCommandLineMovies
// Parse the command line when the application first starts up and
// open as movie documents any files specified on the command line.
//
// Based on the routine ParseCmdLinePriv in GraphicImporter.c.
//
//////////

void QTFrame_OpenCommandLineMovies(LPSTR theCmdLine)
{
#pragma unused(theCmdLine)
	LPSTR myCmdLine;
	FSSpec myFSSpec;
	SHFILEINFO myFileInfo;

	// get the command line for the current process
	myCmdLine = GetCommandLineA();

	// parse the command line
	if (*myCmdLine) {
		LPSTR myTempLine;

		// the string preceding any white space is the name of the module (that
		// is, the application)
		myTempLine = strchr(myCmdLine, ' ');
		if (myTempLine) {
			myCmdLine = myTempLine; // skip the name of the application
			while (*myCmdLine == ' ')
				myCmdLine++; // skip spaces to end of string or to first command

			while (*myCmdLine != '\0') {
				char myFileName[MAX_PATH];
				char myTempName[MAX_PATH];
				char myBuffName[MAX_PATH];
				int myIndex;

				// read thru the remaining string to find file names
				for (myIndex = 0; *myCmdLine != '\0'; myIndex++, myCmdLine++) {
					// if we encounter a space character, it might be a filename
					// delimiter or a space in the filename; we'll try to open
					// the filename we have so far to see whether it's a valid
					// filename; if not, the space must be part of the filename
					// we're constructing
					if (*myCmdLine == ' ') {
						HANDLE myFindFile;
						WIN32_FIND_DATA myFile;

						myTempName[myIndex] = '\0';
						strcpy(myBuffName, myTempName);

						myFindFile = FindFirstFileA(myBuffName, &myFile);
						if (myFindFile != INVALID_HANDLE_VALUE) {
							// we found a file having the specified name; close
							// our file search and break out of our
							// character-gobbling loop (since we've got a valid
							// filename)
							FindClose(myFindFile);
							break;
						}
					}

					// if we made it here, *myCmdLine is part of the filename
					// (possibly a space)
					myFileName[myIndex] = myTempName[myIndex] = *myCmdLine;
				}

				if (*myCmdLine != '\0') {
					myCmdLine++;
				}

				// add a terminating NULL character
				myFileName[myIndex] = '\0';

				// make sure the filename picks out a QuickTime movie
				SHGetFileInfoA(myFileName, (DWORD)0, &myFileInfo,
					sizeof(myFileInfo), SHGFI_TYPENAME);
				if (strcmp(myFileInfo.szTypeName, gMovieType) != 0)
					continue;

				// make an FSSpec record
				NativePathNameToFSSpec(myFileName, &myFSSpec, 0L);

				// open the file in a movie window
				QTFrame_OpenMovieInWindow(NULL, &myFSSpec);
			}

		} else {
			myCmdLine += strlen(myCmdLine); // point to NULL
		}
	}
}

//////////
//
// QTFrame_CreateMovieWindow
// Create a new window to display the movie in.
//
//////////

WindowReference QTFrame_CreateMovieWindow(void)
{
	WindowReference myWindow = NULL;

	gWeAreCreatingWindow = true;

	// create a new window to display the movie in
	myWindow = CreateWindowEx(WS_EX_MDICHILD, gChildName, "", 0, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghWndMDIClient, NULL,
		ghInst, 0);

	// CreateWindowEx sends a WM_CREATE message to the window being created;
	// we'll call QTFrame_CreateWindowObject when processing that message

	gWeAreCreatingWindow = false;

	// make sure the new window has the keyboard focus
	if (myWindow != NULL) {
		SetFocus(myWindow);
	}

	return myWindow;
}

//////////
//
// QTFrame_DestroyMovieWindow
// Close the specified movie window.
//
//////////

void QTFrame_DestroyMovieWindow(WindowReference theWindow)
{
#pragma unused(theWindow)

	HWND myChild = NULL;

	// close the active child window
	myChild = (HWND)SendMessage(ghWndMDIClient, WM_MDIGETACTIVE, 0, 0L);
	if (IsWindow(myChild))
		SendMessage(myChild, WM_CLOSE, 0L, 0L);
}

//////////
//
// QTFrame_GetDisplayName
// Given a full pathname, return the part that trails the rightmost path
// separator, in long file name format (not in 8.3 format).
//
//////////

void QTFrame_GetDisplayName(char* thePathName, char* theDispName)
{
	SHFILEINFO myFileInfo;
	DWORD myResult;

	myResult = SHGetFileInfoA(thePathName, (DWORD)0, &myFileInfo,
		sizeof(myFileInfo), SHGFI_DISPLAYNAME);
	if (myResult != 0) {
		// SHGetFileInfoA successful
		strcpy(theDispName, myFileInfo.szDisplayName);
	} else {
		// SHGetFileInfoA not successful, so find the basename ourselves
		short myLength = 0;
		short myIndex;

		// get the length of the pathname
		myLength = (short)strlen(thePathName);

		// find the position of the rightmost path separator in thePathName
		if (strchr(thePathName, kWinFilePathSeparator) != NULL) {

			myIndex = (short)(myLength - 1);
			while (thePathName[myIndex] != kWinFilePathSeparator)
				myIndex--;

			// calculate the length of the basename
			myLength = (short)(myLength - myIndex - 1);

		} else {
			// there is no rightmost path separator in thePathName;
			// set myIndex so that myIndex + 1 == 0, for the call to BlockMove
			// below
			myIndex = -1;
		}

		// copy into theDispName the substring of thePathName from myIndex + 1
		// to the end
		BlockMove(&thePathName[myIndex + 1], theDispName, myLength);
		theDispName[myLength] = '\0';
	}
}

//////////
//
// QTFrame_ShowAboutBox
// Display and manage the About dialog box.
//
//////////

void QTFrame_ShowAboutBox(void)
{
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUT), ghWnd,
		(DLGPROC)QTFrame_DialogProcedure);
}

//////////
//
// QTFrame_ShowCautionAlert
// Display and manage a caution alert.
//
// Based on ShowUserMessage by Stephen Chernicoff, in his WiniEdit application
// (as described in the book "From Mac to Windows" on CodeWarrior reference CD).
//
//////////

int QTFrame_ShowCautionAlert(HWND theWnd, UINT theID, UINT theIconStyle,
	UINT theButtonStyle, LPSTR theTitle, LPSTR theArgument)
{
	char myTemplate[kAlertMessageMaxLength];
	char myText[kAlertMessageMaxLength];
	UINT myStyle;
	int myItem;

	// beep, to get the user's attention (just like CautionAlert on MacOS)
	QTFrame_Beep();

	// load the message text template from a resource
	LoadString(ghInst, theID, myTemplate, sizeof(myTemplate));

	// insert argument into the message text template, to get the message text
	wsprintf(myText, myTemplate, theArgument);

	// set the dialog box style
	myStyle = theIconStyle | theButtonStyle | MB_APPLMODAL | MB_SETFOREGROUND;

	// display the dialog box
	myItem = MessageBox(theWnd, myText, theTitle, myStyle);

	return (myItem);
}

//////////
//
// QTFrame_DialogProcedure
// Dialog callback procedure.
//
//////////

static UINT APIENTRY QTFrame_DialogProcedure(
	HWND theDialog, UINT theMessage, WPARAM wParam, LPARAM lParam)
{
	BOOL isHandled = false;

	switch (theMessage) {

	case WM_INITDIALOG: {
		Point myPoint;
		long myWidth;
		long myHeight;
		RECT myRect;
		RECT myDeskRect;
		HWND myWindow;
		OPENFILENAME* myOFNPtr = (OPENFILENAME*)lParam;

		myWindow = theDialog;

		// check whether theDialog is the Open File common dialog box

		// we need to do this because, for the Open File dialog box, theDialog
		// isn't the actual visible dialog box, but an invisible child of the
		// visible dialog box; for WM_INITDIALOG, lParam is the address of the
		// structure passed to GetOpenFileName, so we can just look for the
		// value we previously put into the lCustData field to make sure that
		// we've got the correct dialog.
		if (myOFNPtr != NULL)
			if (myOFNPtr->lCustData == kOpenDialogCustomData)
				myWindow = GetParent(theDialog);

		// center the dialog window on the screen
		GetWindowRect(myWindow, &myRect);
		myWidth = myRect.right - myRect.left;
		myHeight = myRect.bottom - myRect.top;
		GetWindowRect(GetDesktopWindow(), &myDeskRect);
		myPoint.h =
			(short)((myDeskRect.right + myDeskRect.left) / 2 - myWidth / 2);
		myPoint.v =
			(short)((myDeskRect.top + myDeskRect.bottom) / 3 - myHeight / 3);
		SetWindowPos(
			myWindow, 0, myPoint.h, myPoint.v, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		// let Windows set the input focus
		isHandled = true;
		break;
	}

	case WM_CLOSE:
		EndDialog(theDialog, IDOK);
		isHandled = true;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(theDialog, IDOK);
			isHandled = true;
			break;
		default:
			isHandled = false;
			break;
		}
		break;

	default:
		isHandled = false;
		break;
	}

	return (UINT)isHandled;
}

//////////
//
// QTFrame_CalcWindowMinMaxInfo
// Get minimum and maximum possible size of this window.
//
//////////

static void QTFrame_CalcWindowMinMaxInfo(HWND theWnd, LPMINMAXINFO lpMinMax)
{
	WindowObject myWindowObject = NULL;
	Movie myMovie = NULL;
	MovieController myMC = NULL;
	GraphicsImportComponent myImporter = NULL;
	Rect myRect;
	ComponentResult myErr = noErr;

	myRect.top = 0;
	myRect.left = 0;
	myRect.bottom = 0;
	myRect.right = 0;

	myWindowObject = QTFrame_GetWindowObjectFromWindow(theWnd);
	if (myWindowObject != NULL) {
		myMC = (**myWindowObject).fController;
		myMovie = (**myWindowObject).fMovie;
		myImporter = (**myWindowObject).fGraphicsImporter;
	}

	// we're expecting a window with either an image or a movie
	if ((myImporter == NULL) && (myMovie == NULL))
		return;

	if (myImporter != NULL) {
		myErr = GraphicsImportGetBoundsRect(myImporter, &myRect);
		if (myErr != noErr)
			return;
	}

	if (myMovie != NULL) {
		GetMovieBox(myMovie, &myRect);
		if (myMC != NULL)
			if (MCGetVisible(myMC))
				MCGetControllerBoundsRect(myMC, &myRect);
	}

	// we currently don't allow images or movies to be resized, so we return
	// both min and max rectangles set to the current image or movie size, plus
	// the appropriate window frame and caption sizes
	lpMinMax->ptMinTrackSize.x =
		myRect.right + (2 * GetSystemMetrics(SM_CXFRAME));
	lpMinMax->ptMinTrackSize.y = myRect.bottom +
		(2 * GetSystemMetrics(SM_CXFRAME)) + GetSystemMetrics(SM_CYCAPTION);

	lpMinMax->ptMaxTrackSize.x = lpMinMax->ptMinTrackSize.x;
	lpMinMax->ptMaxTrackSize.y = lpMinMax->ptMinTrackSize.y;

	lpMinMax->ptMaxSize.x = lpMinMax->ptMinTrackSize.x;
	lpMinMax->ptMaxSize.y = lpMinMax->ptMinTrackSize.y;
}

//////////
//
// QTFrame_GetKeyboardModifiers
// Get the current keyboard modifiers; return a long word encoding those
// modifiers that's in the same format as the modifiers field of an event
// record.
//
// The Windows equivalent of the Option key is defined as the combination of
// Ctrl and Alt; so, if they are both down, we should only return the Option
// key; if only one of them is down, then we return that one.
//
// Based on code from Brian Friedkin.
//
//////////

static long QTFrame_GetKeyboardModifiers(void)
{
	long myModifiers = 0L;
	BYTE myKeyState[256];

	if (GetKeyboardState(&myKeyState[0])) {
		if (QTFrame_IsOptionKeyDown(myKeyState))
			myModifiers |= optionKey;
		else if (QTFrame_IsCommandKeyDown(myKeyState))
			myModifiers |= cmdKey;
		else if (QTFrame_IsControlKeyDown(myKeyState))
			myModifiers |= controlKey;

		if (QTFrame_IsShiftKeyDown(myKeyState))
			myModifiers |= shiftKey;
		if (QTFrame_IsAlphaLockKeyDown(myKeyState))
			myModifiers |= alphaLock;
	}

	return (myModifiers);
}
