//
// Resource fork for the QTTimeCode sample
//

#include <MacTypes.r>
#include <Dialogs.r>
#include <Menus.r>
#include <Controls.r>
#include <Finder.r>
#include <Icons.r>

#include "ComResource.h"

// Note 0xA9 is Copyright
// 0xC9 is ...

// Needed for the owner resource
type 'QTtc' {
	pstring;
};

// Owner resource creator code
resource 'QTtc' (0, "Owner resource") {
	"1998 by Apple Computer, Inc."
};

// Version 1.1 (A9 is the copyright symbol in Mac Roman US)
resource 'vers' (1) {
	1,				// Major version
	0x10,			// Minor version
	development,	// development,alpha,beta,final
	0,				// non-final number
	verUS,			// region code
	"1.1",
	"1.1, \0xA9 Apple Computer, Inc. 2000"
};

resource 'vers' (2) {
	1,				// Major version
	0x10,			// Minor version
	development,	// development,alpha,beta,final
	0,				// non-final number
	verUS,			// region code
	"1.1",
	"QuickTime 4.1"
};

// Finder bundles
resource 'BNDL' (128) {
	'QTtc',
	0,
	{
		'FREF',
		{
			0, 130,
			1, 131
		},
		'ICN#',
		{
			0, 129,
			1, 128
		}
	}
};

resource 'FREF' (128) {
	'APPL', 0, ""	// Application
};

resource 'FREF' (129) {
	'MooV', 1, ""	// Movie file
};

resource 'FREF' (130) {
	'MooV', 0, ""	// Movie file
};

resource 'FREF' (131) {
	'APPL',	1, ""	// Application
};

// Name of the app
resource 'STR#' (1000) {
	{ "QTTimeCode" }
};

// Main menu bar
resource 'MBAR' (kMenuBarResID) {
	{
		kAppleMenuResID, kFileMenuResID, kEditMenuResID, kTestMenuResID
	}
};

// Menu items

resource 'MENU' (kAppleMenuResID) {
	kAppleMenuResID,
	textMenuProc,
	0x7FFFFFFF & (~0x2),
	enabled,
	apple,
	{	
		"About QTTimeCode\0xC9", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (kFileMenuResID) {
	kFileMenuResID,
	textMenuProc,
	0x7FFFFFFF & (~0x20),
	enabled,
	"File",
	{
		"New", noIcon, "N", noMark, plain,
		"Open\0xC9", noIcon, "O", noMark, plain,
		"Close", noIcon, "W", noMark, plain,
		"Save", noIcon, "S", noMark, plain,
		"Save As\0xC9", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (kEditMenuResID) {
	kEditMenuResID,
	textMenuProc,
	0x7FFFFFFF & ~(0x40 | 0x2),
	enabled,
	"Edit",
	{
		"Undo", noIcon, "Z", noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Cut", noIcon, "X", noMark, plain,
		"Copy", noIcon, "C", noMark, plain,
		"Paste", noIcon, "V", noMark, plain,
		"Clear", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Select All", noIcon, "A", noMark, plain,
		"Select None", noIcon, "B", noMark, plain
	}
};

resource 'MENU' (kTestMenuResID) {
	kTestMenuResID,
	textMenuProc,
	0x7FFFFFFF & ~(0x4 | 0x20),
	enabled,
	"Test",
	{
		"Add Timecode Track\0xC9", noIcon, "1", noMark, plain,
		"Remove Timecode Tracks", noIcon, "2", noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Show Current Timecode", noIcon, "3", noMark, plain,
		"Show Timecode Source", noIcon, "4", noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Toggle Timecode Display", noIcon, "5", noMark, plain
	}
};

// Dialogs

// About box
resource 'DLOG' (128, purgeable) {
	{219, 343, 379, 703},
	movableDBoxProc,
	visible,
	noGoAway,
	0,
	128,
	"About QTTimeCode",
	alertPositionMainScreen
};

resource 'dlgx' (128, purgeable) {
	versionZero {
		kDialogFlagsUseThemeBackground
	}
};


resource 'DITL' (128, purgeable) {
	{
		{127, 287, 147, 347},
		Button {
			enabled, "OK"
		},

		{13, 23, 114, 99},
		Picture {
			disabled, 128	// See the PICT below
		},

		{13, 122, 114, 348},
		StaticText {
			disabled,
			"Using the timecode media handler.\n\n"
			"Written by the QuickTime Team\n"
			"\0xA92000 Apple Computer, Inc."
		},

		{127, 287, 147, 347},
		UserItem {
			disabled
		}
	}
};

// This is a PICT 1 format file
read 'PICT' (128, "About box PICT") "Penguin.pict";

// Options dialog

resource 'DLOG' (200, "TimeCode Options") {
	{40, 40, 336, 433},
	movableDBoxProc,
	invisible,
	goAway,
	0,
	200,
	"Timecode Options",
	centerMainScreen
};

resource 'dlgx' (200, purgeable) {
	versionZero {
		kDialogFlagsUseThemeBackground
	}
};

resource 'DITL' (200, "TimeCode Options") {
	{
		{264, 303, 284, 373},
		Button {
			enabled, "OK"
		},

		{264, 226, 284, 291},
		Button {
			enabled, "Cancel"
		},

		{10, 30, 26, 125},
		StaticText {
			disabled, "Source Name"
		},

		{11, 140, 27, 376},
		EditText {
			enabled, "Edit Text"
		},

		{35, 29, 51, 190},
		CheckBox {
			enabled, "Display TimeCode"
		},

		{59, 28, 76, 109},
		StaticText {
			disabled, "Time scale"
		},

		{60, 161, 75, 264},
		EditText {
			enabled, "Edit Text"
		},

		{83, 28, 99, 139},
		StaticText {
			disabled, "Frame duration"
		},

		{83, 161, 99, 264},
		EditText {
			enabled, "Edit Text"
		},

		{106, 28, 122, 153},
		StaticText {
			disabled, "Number of frames"
		},

		{107, 161, 122, 264},
		EditText {
			enabled, "Edit Text"
		},

		{141, 50, 157, 145},
		RadioButton {
			enabled, "TimeCode"
		},

		{141, 200, 157, 295},
		RadioButton {
			enabled, "Counter"
		},

		{169, 49, 185, 144},
		CheckBox {
			enabled, "DropFrame"
		},

		{184, 49, 200, 152},
		CheckBox {
			enabled, "24 Hour Max"
		},

		{199, 49, 215, 144},
		CheckBox {
			enabled, "Neg OK"
		},

		{224, 46, 243, 164},
		StaticText {
			disabled, "HH : MM : SS : FF"
		},

		{241, 20, 267, 37},
		UserItem {
			enabled
		},

		{244, 45, 264, 65},
		EditText {
			enabled, "00"
		},

		{244, 76, 264, 96},
		EditText {
			enabled, "00"
		},

		{244, 107, 264, 127},
		EditText {
			enabled, "00"
		},

		{244, 138, 264, 158},
		EditText {
			enabled, "00"
		},

		{170, 216, 186, 277},
		StaticText {
			disabled, "Counter"
		},

		{191, 197, 207, 300},
		EditText {
			enabled, "Edit Text"
		},

		{35, 216, 52, 328},
		CheckBox {
			enabled, "Below Video"
		},

		{227, 170, 247, 355},
		Control {
			enabled, 128
		}
	}
};

// Font control
resource 'CNTL' (128) {
	{220, 180, 247, 400},
	0,					// Style Code
	visible,			// visible, invisble
	40,					// Title Width
	1000,				// Menu ID
	popupMenuCDEFproc,	// ProcID
	0,					// Refcon
	"Font: "			// Title
};

// Font selection menu control
resource 'MENU' (1000) {
	1000,
	textMenuProc,
	allEnabled,
	enabled,
	"Font",
	{
	}
};


// Show Alert
resource 'ALRT' (201, "Show") {
	{40, 40, 180, 340},
	201,
	{
		OK, visible, silent,
		OK, visible, silent,
		OK, visible, silent,
		OK, visible, silent
	},
	alertPositionMainScreen
};

// Icons

// 1 bit icons

resource 'ics#' (128) {
	{
		$"0100 0380 0540 0E20 1410 3808 5004 E00E"
		$"4014 2038 1050 08E0 0540 0380 0100 0000",

		$"0100 0380 07C0 0FE0 1FF0 3FF8 7FFC FFFE"
		$"7FFC 3FF8 1FF0 0FE0 07C0 0380 0100 0000"
	}
};

resource 'ics#' (129, "Movie Document Icon") {
	{
		$"7FE0 7030 5028 703C 5014 7FFC 5014 701C"
		$"5014 701C 5FF4 701C 5014 701C 5014 7FFC",

		$"7FE0 7FF0 7FF8 7FFC 7FFC 7FFC 7FFC 7FFC"
		$"7FFC 7FFC 7FFC 7FFC 7FFC 7FFC 7FFC 7FFC"
	}
};

resource 'ics#' (130) {
	{
		$"7FE0 7030 5028 703C 5014 7FFC 5014 701C"
		$"FFF4 BA3C BAD4 D6DC D634 EEDC EED4 FFFC",

		$"7FE0 7FF0 7FF8 7FFC 7FFC 7FFC 7FFC 7FFC"
		$"FFFC FFFC FFFC FFFC FFFC FFFC FFFC FFFC"
	}
};

resource 'ics#' (131) {
	{
		$"0FFF 0BA3 0BAD 0D6D 1D63 3EED 5EED EFFF"
		$"4094 2078 1050 FFFF 8851 8FDF 8851 FFFF",

		$"0FFF 0FFF 0FFF 0FFF 1FFF 3FFF 7FFF FFFF"
		$"7FFC 3FF8 1FF0 FFFF FFFF FFFF FFFF FFFF"
	}
};

// Greyscale icons
resource 'ICN#' (128) {
	{
		$"0001 0000 0003 8000 0006 C000 000C 6000"
		$"001E F000 0037 9800 0063 0C00 00F6 0600"
		$"01BC 0300 0318 0180 07B0 00C0 0DE0 0060"
		$"18C0 0030 3D80 0038 6F00 006C C600 00C6"
		$"6C00 01EC 3800 0378 1800 0630 0C00 0F60"
		$"0600 1BC0 FFFF FFFF 8000 0001 9831 8319"
		$"A44A 44A5 A54A 54A5 A44A 44A5 A54A 54A5"
		$"A44A 44A5 9831 8319 8000 0001 FFFF FFFF",

		$"0001 0000 0003 8000 0007 C000 000F E000"
		$"001F F000 003F F800 007F FC00 00FF FE00"
		$"01FF FF00 03FF FF80 07FF FFC0 0FFF FFE0"
		$"1FFF FFF0 3FFF FFF8 7FFF FFFC FFFF FFFE"
		$"7FFF FFFC 3FFF FFF8 1FFF FFF0 0FFF FFE0"
		$"07FF FFC0 FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	}
};

resource 'ICN#' (129, "Movie Document Icon") {
	{
		$"1FFF FC00 1200 0600 1200 0500 1E00 0480"
		$"1E00 0440 1200 0420 1200 07F0 1E00 00F0"
		$"1E00 00F0 1200 0090 13FF FF90 1E00 00F0"
		$"1E00 00F0 1200 0090 1200 0090 1E00 00F0"
		$"1E00 00F0 1200 0090 1200 0090 1E00 00F0"
		$"1E00 00F0 13FF FF90 1200 0090 1E00 00F0"
		$"1E00 00F0 1200 0090 1200 0090 1E00 00F0"
		$"1E00 00F0 1200 0090 1200 0090 1FFF FFF0",

		$"1FFF FC00 1FFF FE00 1FFF FF00 1FFF FF80"
		$"1FFF FFC0 1FFF FFE0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
		$"1FFF FFF0 1FFF FFF0 1FFF FFF0 1FFF FFF0"
	}
};

// Small 4 bit color icons

resource 'ics4' (130) {
	$"0FFF FFFF FFF0 0000 0FFF 0C0C 0CFF 0000"
	$"0F0F C0C0 C0FC F000 0FFF 0C0C 0CFF FF00"
	$"0F0F C0C0 C0CF 0F00 0FFF FFFF FFFF FF00"
	$"0F0F C0C0 C0CF 0F00 0FFF 0C0C 0C0F FF00"
	$"FFFF FFFF FFFF 0F00 F1FF F1F1 11FF FF00"
	$"F1FF F1F1 FF1F 0F00 FF1F 1FF1 FF1F FF00"
	$"FF1F 1FF1 11FF 0F00 FFF1 FFF1 FF1F FF00"
	$"FFF1 FFF1 FF1F 0F00 FFFF FFFF FFFF FF"
};

resource 'ics4' (131) {
	$"0000 FFFF FFFF FFFF 0000 F1FF F1F1 11FF"
	$"0000 F1FF F1F1 FF1F 0000 FF1F 1FF1 FF1F"
	$"000F FF1F 1FF1 11FF 00FF FFF1 FFF1 FF1F"
	$"0F0F FFF1 FFF1 FF1F FFFC FFFF FFFF FFFF"
	$"0FC0 C0C0 F0CF 0F00 00FC 0C0C 0FFF F000"
	$"000F C0C0 CF0F 0000 FFFF FFFF FFFF FFFF"
	$"F000 FCCC CF0F CCCF F000 FEEE EF0F EEEF"
	$"F000 FCCC CF0F CCCF FFFF FFFF FFFF FFFF"
};

// Large 4 bit color icons

resource 'icl4' (130) {
	$"000F FFFF FFFF FFFF FFFF FF00 0000 0000"
	$"000F 00FC 0C0C 0C0C 0C0C 0FF0 0000 0000"
	$"000F 00F0 C0C0 C0C0 C0C0 CFCF 0000 0000"
	$"000F FFFC 0C0C 0C0C 0C0C 0FCC F000 0000"
	$"000F FFF0 C0C0 C0C0 C0C0 CFCC CF00 0000"
	$"000F 00FC 0C0C 0C0C 0C0C 0FCC CCF0 0000"
	$"000F 00F0 C0C0 C0C0 C0C0 CFFF FFFF 0000"
	$"000F FFFC 0C0C 0C0C 0C0C 0C0C FFFF 0000"
	$"000F FFF0 C0C0 C0C0 C0C0 C0C0 FFFF 0000"
	$"000F 00FC 0C0C 0C0C 0C0C 0C0C F00F 0000"
	$"000F 00FF FFFF FFFF FFFF FFFF F00F 0000"
	$"000F FFFC 0C0C 0C0C 0C0C 0C0C FFFF 0000"
	$"000F FFF0 C0C0 C0C0 C0C0 C0C0 FFFF 0000"
	$"000F 00FC 0C0C 0C0C 0C0C 0C0C F00F 0000"
	$"000F 00F0 C0C0 C0C0 C0C0 C0C0 F00F 0000"
	$"000F FFFC 0C0C 0C0C 0C0C 0C0C FFFF 0000"
	$"000F FFF0 C0C0 C0C0 C0C0 C0C0 FFFF 0000"
	$"000F 00FC 0C0C 0C0C 0C0C 0C0C F00F 0000"
	$"000F 00F0 C0C0 C0C0 C0C0 C0C0 F00F 0000"
	$"000F FFFC 0C0C 0C0C 0C0C 0C0C FFFF 0000"
	$"000F FFF0 C0C0 C0C0 C0C0 C0C0 FFFF 0000"
	$"000F 00FF FFFF FFFF FFFF FFFF F00F 0000"
	$"EEEE EEEE EEEE EEC0 C0C0 C0C0 F00F 0000"
	$"EFFF FFFF FFFF FE0C 0C0C 0C0C FFFF 0000"
	$"EF1F FF1F 111F FEC0 C0C0 C0C0 FFFF 0000"
	$"EF1F FF1F 1FF1 FE0C 0C0C 0C0C F00F 0000"
	$"EFF1 F1FF 1FF1 FEC0 C0C0 C0C0 F00F 0000"
	$"EFF1 F1FF 111F FE0C 0C0C 0C0C FFFF 0000"
	$"EFFF 1FFF 1FF1 FEC0 C0C0 C0C0 FFFF 0000"
	$"EFFF 1FFF 1FF1 FE0C 0C0C 0C0C F00F 0000"
	$"EFFF FFFF FFFF FEC0 C0C0 C0C0 F00F 0000"
	$"EEEE EEEE EEEE EEFF FFFF FFFF FFFF 0000"
};

resource 'icl4' (131) {
	$"0000 0000 0000 000F 00EE EEEE EEEE EEEE"
	$"0000 0000 0000 00FF F0EF FFFF FFFF FFFE"
	$"0000 0000 0000 0FF0 FFEF 1FFF 1F11 1FFE"
	$"0000 0000 0000 FF00 0FEF 1FFF 1F1F F1FE"
	$"0000 0000 000F FFF0 FFEF F1F1 FF1F F1FE"
	$"0000 0000 00FF 0FFF FCEF F1F1 FF11 1FFE"
	$"0000 0000 0FF0 00FF C0EF FF1F FF1F F1FE"
	$"0000 0000 FFFF 0FFC 0CEF FF1F FF1F F1FE"
	$"0000 000F F0FF FFC0 C0EF FFFF FFFF FFFE"
	$"0000 00FF 000F FC0C 0CEE EEEE EEEE EEEE"
	$"0000 0FFF F0FF C0C0 C0C0 C0C0 FF00 0000"
	$"0000 FF0F FFFF 0C0C 0C0C 0C0C 0FF0 0000"
	$"000F F000 FFCF F0C0 C0C0 C0C0 C0FF 0000"
	$"00FF FF0F FC0C FF0C 0C0C 0C0C 0CFF F000"
	$"0FF0 FFFF C0C0 CFF0 C0C0 C0C0 CFF0 FF00"
	$"FF00 0FFC 0C0C 0CFF 0C0C 0C0C FF00 0FF0"
	$"0FF0 FFC0 C0C0 C0CF F0C0 C0CF FFF0 FF00"
	$"00FF FC0C 0C0C 0C0C FF0C 0CFF 0FFF F000"
	$"000F F0C0 C0C0 C0C0 CFF0 CFF0 00FF 0000"
	$"0000 FF0C 0C0C 0C0C 0CFF FFFF 0FF0 0000"
	$"0000 0FF0 C0C0 C0C0 C0CF F0FF FF00 0000"
	$"0000 00FF 0C0C 0C0C 0CFF 000F F000 0000"
	$"0000 000F F0C0 C0C0 CFFF F0FF 0000 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"F000 0000 F000 0000 0000 F000 F000 000F"
	$"F00F 0000 FFFF FFFF FFFF F000 FFFF FFFF"
	$"F00F F000 FDDD DDDD DDDD F000 FDDD DDDF"
	$"F00F FF00 FDDD DDDD DDDD F000 FDDD DDDF"
	$"F00F F000 FDDD DDDD DDDD F000 FDDD DDDF"
	$"F00F 0000 FFFF FFFF FFFF F000 FFFF FFFF"
	$"F000 0000 F000 0000 0000 F000 F000 000F"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

// Small 8 bit icons

resource 'ics8' (130) {
	$"00FF FFFF FFFF FFFF FFFF FF00 0000 0000"
	$"00FF FFFF F5F5 F5F5 F5F5 FFFF 0000 0000"
	$"00FF 00FF F5F5 F5F5 F5F5 FF2B FF00 0000"
	$"00FF FFFF F5F5 F5F5 F5F5 FFFF FFFF 0000"
	$"00FF 00FF F5F5 F5F5 F5F5 F5FF 00FF 0000"
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"00FF 00FF F5F5 F5F5 F5F5 F5FF 00FF 0000"
	$"00FF FFFF F5F5 F5F5 F5F5 F5FF FFFF 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 00FF 0000"
	$"FF05 FFFF FF05 FF05 0505 FFFF FFFF 0000"
	$"FF05 FFFF FF05 FF05 FFFF 05FF 00FF 0000"
	$"FFFF 05FF 05FF FF05 FFFF 05FF FFFF 0000"
	$"FFFF 05FF 05FF FF05 0505 FFFF 00FF 0000"
	$"FFFF FF05 FFFF FF05 FFFF 05FF FFFF 0000"
	$"FFFF FF05 FFFF FF05 FFFF 05FF 00FF 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF 0000"
};

resource 'ics8' (131) {
	$"0000 0000 FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 FF05 FFFF FF05 FF05 0505 FFFF"
	$"0000 0000 FF05 FFFF FF05 FF05 FFFF 05FF"
	$"0000 0000 FFFF 05FF 05FF FF05 FFFF 05FF"
	$"0000 00FF FFFF 05FF 05FF FF05 0505 FFFF"
	$"0000 FFFF FFFF FF05 FFFF FF05 FFFF 05FF"
	$"00FF 00FF FFFF FF05 FFFF FF05 FFFF 05FF"
	$"FFFF FF2A FFFF FFFF FFFF FFFF FFFF FFFF"
	$"00FF 2A2A 2A2A 2A2A FF2A 2AFF 00FF 0000"
	$"0000 FF2A 2A2A 2A2A 2AFF FFFF FF00 0000"
	$"0000 00FF 2A2A 2A2A 2AFF 00FF 0000 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FF00 0000 FFF6 F6F6 F6FF 00FF F6F6 F6FF"
	$"FF00 0000 FFFB FBFB FBFF 00FF FBFB FBFF"
	$"FF00 0000 FFF6 F6F6 F6FF 00FF F6F6 F6FF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

// Large 8 bit color icons

resource 'icl8' (130) {
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF 0000 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FF00 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2BFF 0000 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B FF00 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2BFF 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2B2B FF00 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FFFF FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F500 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 0000 0000 0000 0000"
	$"0000 0000 0000 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FF00 00FF 0000 0000"
	$"A5A5 A5A5 A5A5 A5A5 A5A5 A5A5 A5A5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"A5FF FFFF FFFF FFFF FFFF FFFF FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"A5FF 05FF FFFF 05FF 0505 05FF FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"A5FF 05FF FFFF 05FF 05FF FF05 FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"A5FF FF05 FF05 FFFF 05FF FF05 FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"A5FF FF05 FF05 FFFF 0505 05FF FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"A5FF FFFF 05FF FFFF 05FF FF05 FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"A5FF FFFF 05FF FFFF 05FF FF05 FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"A5FF FFFF FFFF FFFF FFFF FFFF FFA5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"A5A5 A5A5 A5A5 A5A5 A5A5 A5A5 A5A5 FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 0000"
};

resource 'icl8' (131) {
	$"0000 0000 0000 0000 0000 0000 0000 00FF"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 0000 0000 0000 0000 0000 FFFF"
	$"FF00 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 0000 0000 0000 0000 00FF FF00"
	$"FFFF FFFF 05FF FFFF 05FF 0505 05FF FFFF"
	$"0000 0000 0000 0000 0000 0000 FFFF 0000"
	$"00FF FFFF 05FF FFFF 05FF 05FF FF05 FFFF"
	$"0000 0000 0000 0000 0000 00FF FFFF FF00"
	$"FFFF FFFF FF05 FF05 FFFF 05FF FF05 FFFF"
	$"0000 0000 0000 0000 0000 FFFF 00FF FFFF"
	$"FF2A FFFF FF05 FF05 FFFF 0505 05FF FFFF"
	$"0000 0000 0000 0000 00FF FF00 0000 FFFF"
	$"2A2A FFFF FFFF 05FF FFFF 05FF FF05 FFFF"
	$"0000 0000 0000 0000 FFFF FFFF 00FF FF2A"
	$"2A2A FFFF FFFF 05FF FFFF 05FF FF05 FFFF"
	$"0000 0000 0000 00FF FF00 FFFF FFFF 2A2A"
	$"2A2A FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 0000 FFFF 0000 00FF FF2A 2A2A"
	$"2A2A FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 00FF FFFF FF00 FFFF 2A2A 2A2A"
	$"2A2A 2A2A 2A2A 2A2A FFFF 0000 0000 0000"
	$"0000 0000 FFFF 00FF FFFF FFFF 2A2A 2A2A"
	$"2A2A 2A2A 2A2A 2A2A 2AFF FF00 0000 0000"
	$"0000 00FF FF00 0000 FFFF 2AFF FF2A 2A2A"
	$"2A2A 2A2A 2A2A 2A2A 2A2A FFFF 0000 0000"
	$"0000 FFFF FFFF 00FF FF2A 2A2A FFFF 2A2A"
	$"2A2A 2A2A 2A2A 2A2A 2A2A FFFF FF00 0000"
	$"00FF FF00 FFFF FFFF 2A2A 2A2A 2AFF FF2A"
	$"2A2A 2A2A 2A2A 2A2A 2AFF FF00 FFFF 0000"
	$"FFFF 0000 00FF FF2A 2A2A 2A2A 2A2A FFFF"
	$"2A2A 2A2A 2A2A 2A2A FFFF 0000 00FF FF00"
	$"00FF FF00 FFFF 2A2A 2A2A 2A2A 2A2A 2AFF"
	$"FF2A 2A2A 2A2A 2AFF FFFF FF00 FFFF 0000"
	$"0000 FFFF FF2A 2A2A 2A2A 2A2A 2A2A 2A2A"
	$"FFFF 2A2A 2A2A FFFF 00FF FFFF FF00 0000"
	$"0000 00FF FF2A 2A2A 2A2A 2A2A 2A2A 2A2A"
	$"2AFF FF2A 2AFF FF00 0000 FFFF 0000 0000"
	$"0000 0000 FFFF 2A2A 2A2A 2A2A 2A2A 2A2A"
	$"2A2A FFFF FFFF FFFF 00FF FF00 0000 0000"
	$"0000 0000 00FF FF2A 2A2A 2A2A 2A2A 2A2A"
	$"2A2A 2AFF FF00 FFFF FFFF 0000 0000 0000"
	$"0000 0000 0000 FFFF 2A2A 2A2A 2A2A 2A2A"
	$"2A2A FFFF 0000 00FF FF00 0000 0000 0000"
	$"0000 0000 0000 00FF FF2A 2A2A 2A2A 2A2A"
	$"2AFF FFFF FF00 FFFF 0000 0000 0000 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FF00 0000 0000 0000 FF00 0000 0000 0000"
	$"0000 0000 FF00 0000 FF00 0000 0000 00FF"
	$"FF00 00FF 0000 0000 FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FF00 0000 FFFF FFFF FFFF FFFF"
	$"FF00 00FF FF00 0000 FFF9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 FF00 0000 FFF9 F9F9 F9F9 F9FF"
	$"FF00 00FF FFFF 0000 FFF9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 FF00 0000 FFF9 F9F9 F9F9 F9FF"
	$"FF00 00FF FF00 0000 FFF9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 FF00 0000 FFF9 F9F9 F9F9 F9FF"
	$"FF00 00FF 0000 0000 FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FF00 0000 FFFF FFFF FFFF FFFF"
	$"FF00 0000 0000 0000 FF00 0000 0000 0000"
	$"0000 0000 FF00 0000 FF00 0000 0000 00FF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

resource 'icl8' (132) {
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF 0000 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FF00 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2BFF 0000 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B FF00 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2BFF 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2B2B FF00 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FFFF FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"4848 4848 4848 4848 4848 4848 4848 4848"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 C0C0 C0C0 C0C0 C0C0 C0C0 C0C0 C0B0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5B0 F5F5 F5B0 F5B0 B0B0 F5F5 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5B0 F5F5 F5B0 F5B0 F5F5 B0F5 ECB0"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"48C0 F5F5 B0F5 B0F5 F5B0 F5F5 B0F5 ECB0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5F5 B0F5 B0F5 F5B0 B0B0 F5F5 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5F5 F5B0 F5F5 F5B0 F5F5 B0F5 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5F5 F5B0 F5F5 F5B0 F5F5 B0F5 ECB0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 ECB0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 ECEC ECEC ECEC ECEC ECEC ECEC ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48B0 B0B0 B0B0 B0B0 B0B0 B0B0 B0B0 B0B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 0000"
};

resource 'icl8' (133) {
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF 0000 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FF00 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2BFF 0000 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B FF00 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2BFF 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2B2B FF00 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FFFF FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0808 0808 0808 0808 0808 0808 0808 0808"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 1313 1313 1313 1313 1313 1313 13B0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 69B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5B0 F5F5 F5B0 F5B0 B0B0 F5F5 69B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5B0 F5F5 F5B0 F5B0 F5F5 B0F5 69B0"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"0813 F5F5 B0F5 B0F5 F5B0 F5F5 B0F5 69B0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5F5 B0F5 B0F5 F5B0 B0B0 F5F5 69B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5F5 F5B0 F5F5 F5B0 F5F5 B0F5 69B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5F5 F5B0 F5F5 F5B0 F5F5 B0F5 69B0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 69B0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"0813 6969 6969 6969 6969 6969 6969 69B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"08B0 B0B0 B0B0 B0B0 B0B0 B0B0 B0B0 B0B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 0000"
};

resource 'icl8' (134) {
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF 0000 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FF00 0000 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2BFF 0000 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B FF00 0000 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2BFF 0000 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF 2B2B 2B2B FF00 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5FF FFFF FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"4848 4848 4848 4848 4848 4848 4848 4848"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 C0C0 C0C0 C0C0 C0C0 C0C0 C0C0 C0B0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 5454 5454 5454 5454 5454 5454 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 54B0 5454 54B0 54B0 B0B0 5454 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 54B0 5454 54B0 54B0 5454 B054 ECB0"
	$"0000 00FF 0000 FFFF FFFF FFFF FFFF FFFF"
	$"48C0 5454 B054 B054 54B0 5454 B054 ECB0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 5454 B054 B054 54B0 B0B0 5454 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 5454 54B0 5454 54B0 5454 B054 ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 5454 54B0 5454 54B0 5454 B054 ECB0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 5454 5454 5454 5454 5454 5454 ECB0"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48C0 ECEC ECEC ECEC ECEC ECEC ECEC ECB0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"48B0 B0B0 B0B0 B0B0 B0B0 B0B0 B0B0 B0B0"
	$"0000 00FF FFFF FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FFFF FFFF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF 0000 FFF5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 FF00 00FF 0000 0000"
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 0000"
};

