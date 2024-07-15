//////////
//
//	File:		ComResource.h
//
//	Contains:	Resource definitions for the QuickTime sample code framework.
//
//	Written by:	Tim Monroe
//
//	Copyright:	(c) 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <1>	 	11/08/99	rtm		first file
//
//////////

//////////
//
// menu IDs
//
// Windows accesses menu items with a single "menu item identifier", which is of
// type UINT. Macintosh accesses menu items with both a menu ID and a menu item
// index. To be able to access menu items in a cross-platform manner, we will
// map the pair of numbers associated with a Macintosh menu item into a single
// "menu item identifier", using the MENU_IDENTIFIER macro defined in the file
// ComFramework.h. This means that the menu item identifiers that we use in our
// Windows code depend on the values we use for the menu ID and menu item
// indices in the Macintosh resource file.
//
//////////

// resource IDs for Macintosh menus
#define kMenuBarResID 128
#define kAppleMenuResID 128
#define kFileMenuResID 129
#define kEditMenuResID 130
#define kTestMenuResID 131

// IDs for Apple menu items
#define kAboutMenuItem 1

// IDs for File and Edit menus and menu items
#define IDS_FILEMENU 33024 // ((kFileMenuResID<<8)+(0))
#define IDM_FILENEW 33025
#define IDM_FILEOPEN 33026
#define IDM_FILECLOSE 33027
#define IDM_FILESAVE 33028
#define IDM_FILESAVEAS 33029
#define IDM_EXIT 33031

#define IDS_EDITMENU 33280 // ((kEditMenuResID<<8)+(0))
#define IDM_EDITUNDO 33281
#define IDM_EDITCUT 33283
#define IDM_EDITCOPY 33284
#define IDM_EDITPASTE 33285
#define IDM_EDITCLEAR 33286
#define IDM_EDITSELECTALL 33288
#define IDM_EDITSELECTNONE 33289
#define IDM_PREFERENCES 33291

#define IDS_TESTMENU 33536                // ((kTestMenuResID<<8)+(0))
#define IDM_ADD_TIMECODE 33537            // ((kTestMenuResID<<8)+(1))
#define IDM_DELETE_TIMECODE 33538         // ((kTestMenuResID<<8)+(2))
#define IDM_GET_TIMECODE 33540            // ((kTestMenuResID<<8)+(4))
#define IDM_GET_TIMECODE_SOURCE 33541     // ((kTestMenuResID<<8)+(5))
#define IDM_TOGGLE_TIMECODE_DISPLAY 33543 // ((kTestMenuResID<<8)+(7))

// IDs for Window menu and menu items (Windows-only)
#define IDS_WINDOWMENU 1300
#define IDM_WINDOWTILE 1301
#define IDM_WINDOWCASCADE 1302
#define IDM_WINDOWCLOSEALL 1303
#define IDM_WINDOWICONS 1304
#define IDM_WINDOWCHILD 1310

// Windows-only resource IDs
#define IDS_HELPMENU 1400
#define IDM_ABOUT 1401
#define IDC_STATIC -1

#define IDS_APPNAME 1
#define IDS_DESCRIPTION 2
#define WINDOWMENU 3
#define IDS_FILTERSTRING 3
#define IDS_SAVEONCLOSE 2000
#define IDS_SAVEONQUIT 2001
#define IDI_APPICON 101
#define IDI_CHILDICON 102
#define IDD_ABOUT 103
