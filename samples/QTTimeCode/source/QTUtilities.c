//////////
//
//	File:		QTUtilities.c
//
//	Contains:	Some utilities for working with QuickTime movies.
//				All utilities start with the prefix "QTUtils_".
//
//	Written by:	Tim Monroe
//				Based on the DTSQTUtilities package by Apple DTS.
//				This began as essentially a subset of that package, revised for
// cross-platform use.
//
//	Copyright:	(c) 1996-1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	<36>	10/08/00	rtm		tweaked QTUtils_ConvertCToPascalString to copy
// at most 255 characters
//
//	<35>	09/29/00	rtm		added QTUtils_IsAutoPlayMovie
//
//	<34>	04/28/00	rtm		fixed bug in QTUtils_AddUserDataTextToMovie (had
// a script system, not a region code)
//
//	<33>	03/08/00	rtm		removed QTUtils_SaveMovie and
// QTUtils_PrintMoviePICT
//
//	<32>	03/02/00	rtm		added QTUtils_SelectNoneMovie
//
//	<31>	01/10/00	rtm		tweaked QTUtils_IsQuickTimeInstalled
//
//	<30>	12/20/99	rtm		reworked QTUtils_IsMovieFile, to first test for
// files of type kQTFileTypeMovie
//
//	<29>	12/09/99	rtm		added QTUtils_PutControllerBarOnTop
//
//	<28>	12/08/99	rtm		reworked controller button functions to support
// custom buttom (its mcFlags constant
// is a "Use" constant and not a "Suppress" constant)
//
//	<27>	11/26/99	rtm		added endian adjustments to
//_GetMovieFileLoopingInfo and _GetWindowPositionFromFile
//
//	<26> 	11/17/99	rtm		added QTUtils_GetWindowPositionFromFile
//
//	<25>	05/19/99	rtm		removed QTUtils_GetMovie
//
//	<24>	05/10/99	rtm		added QTUtils_UpdateMovieVolumeSetting
//
//	<23>	03/22/99	rtm		updated connection speed code to use
// constants/data types now in Movies.h
//
//	<22>	03/11/99	rtm		moved _GetControllerType and _SetControllerType
// from QTVRUtilities to here; added QTUtils_ChangeControllerType
//
//	<21>	02/03/99	rtm		moved non-QTVR-specific utilities from
// QTVRUtilities to here
//
//	<20>	01/26/99	rtm		added QTUtils_ConvertCToPascalString; removed
//	"\p" from any constant strings
//
//	<19>	01/25/99	rtm		#define'd away QTUtils_GetUsersContentRating and
// siblings, since content rating capability is not in latest feature set
//
//	<18>	12/09/98	rtm		added QTUtils_GetUsersContentRating
//
//	<17>	11/18/98	rtm		added QTUtils_GetFrameCount
//
//	<16>	10/27/98	rtm		added QTUtils_MakeMovieLoop
//
//	<15>	09/14/98	rtm		added QTUtils_GetUsersConnectionSpeed and
// QTUtils_SetUsersConnectionSpeed
//
//	<14>	06/24/98	rtm		added QTUtils_GetMaxWindowDepth and
// QTUtils_GetMaxScreenDepth
//
//	<13>	06/04/98	rtm		added QTUtils_DeleteAllReferencesToTrack
//
//	<12>	05/28/98	rtm		added some typecasting to minimize MSDev
// compiler warnings
//
//	<11>	05/19/98	rtm		added QTUtils_MovieHasTimeCodeTrack
//
//	<10>	05/04/98	rtm		added QTUtils_GetTrackName,
// QTUtils_SetTrackName, QTUtils_MakeTrackNameByType, QTUtils_IsImageFile, and
// QTUtils_IsMovieFile
//
//	<9>		02/28/98	rtm		fixed QTUtils_GetMovieFileLoopingInfo and the
// like
//
//	<8>		01/14/98	rtm		added QTUtils_ConvertFloatToBigEndian
//
//	<7>		12/19/97	rtm		added QTUtils_AddUserDataTextToMovie and
// associated routines; added QTUtils_GetMovieFileLoopingInfo and the like
//
//	<6>		11/06/97	rtm		added QTUtils_MakeSampleDescription
//
//	<5>		10/29/97	rtm		modified QTUtils_IsMediaTypeInMovie and similar
// routines to use GetMovieIndTrackType
//
//	<4>		10/27/97	rtm		added QTUtils_HasQuickTimeVideoEffects
//
//	<3>		10/17/97	rtm		added QTUtils_MovieHasSoundTrack
//
//	<2>		09/23/97	rtm		added endian adjustment to
// QTUtils_PrintMoviePICT
//
//	<1>		09/10/97	rtm		first file
//
//////////

//////////
//
// header files
//
//////////

#ifndef __QTUtilities__
#include "QTUtilities.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// General utilities.
//
// Use these functions to get information about the availability/features of
// QuickTime or other services.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////
//
// QTUtils_TrapAvailable
// Check to see whether a given trap is implemented. This is based on IM:
// Operating System Utilities (p. 8-22).
//
//////////

#if TARGET_OS_MAC && !TARGET_API_MAC_CARBON
Boolean QTUtils_TrapAvailable(short theTrapWord)
{
	TrapType myTrapType;
	short myNumToolboxTraps;

	// determine whether this is a Toolbox or an Operating System trap
	if ((theTrapWord & 0x0800) > 0)
		myTrapType = ToolTrap;
	else
		myTrapType = OSTrap;

	if (myTrapType == ToolTrap) {
		theTrapWord = theTrapWord & 0x07FF;

		if (NGetTrapAddress(_InitGraf, ToolTrap) ==
			NGetTrapAddress(0xAA6E, ToolTrap))
			myNumToolboxTraps = 0x0200;
		else
			myNumToolboxTraps = 0x0400;

		if (theTrapWord >= myNumToolboxTraps)
			theTrapWord = _Unimplemented;
	}

	return (NGetTrapAddress(theTrapWord, myTrapType) !=
		NGetTrapAddress(_Unimplemented, ToolTrap));
}
#endif

//////////
//
// QTUtils_IsQuickTimeInstalled
// Is QuickTime installed?
//
//////////

Boolean QTUtils_IsQuickTimeInstalled(void)
{
	long myAttrs;
	OSErr myErr = noErr;

	myErr = Gestalt(gestaltQuickTime, &myAttrs);

	return (myErr == noErr);
}

//////////
//
// QTUtils_IsQuickTimeCFMInstalled
// Are the QuickTime CFM libraries installed?
//
//////////

#if TARGET_CPU_PPC
Boolean QTUtils_IsQuickTimeCFMInstalled(void)
{
	Boolean myQTCFMAvail = false;
	long myAttrs;
	OSErr myErr = noErr;

	// test whether the PowerPC QuickTime glue library is present
	myErr = Gestalt(gestaltQuickTimeFeatures, &myAttrs);
	if (myErr == noErr)
		if (myAttrs & (1L << gestaltPPCQuickTimeLibPresent))
			myQTCFMAvail = true;

	// test whether a function is available (the library is not moved from the
	// Extension folder); this is the trick to be used when testing if a
	// function is available via CFM
	if (!CompressImage) {
		myQTCFMAvail = false;
	}
	return (myQTCFMAvail);
}
#endif

//////////
//
// QTUtils_GetQTVersion
// Get the version of QuickTime installed.
// The high-order word of the returned long integer contains the version number,
// so you can test a version like this:
//
//		if (((QTUtils_GetQTVersion() >> 16) & 0xffff) >= 0x0210)		// we
// require QT 2.1 or greater 			return;
//
//////////

long QTUtils_GetQTVersion(void)
{
	long myVersion = 0L;
	OSErr myErr = noErr;

	myErr = Gestalt(gestaltQuickTime, &myVersion);
	if (myErr == noErr) {
		return (myVersion);
	} else {
		return (0L);
	}
}

//////////
//
// QTUtils_HasQuickTimeVideoEffects
// Does the installed version of QuickTime support video effects?
//
// Note: this is a pretty cheesy way of determining whether video effects are
// available.
//
//////////

Boolean QTUtils_HasQuickTimeVideoEffects(void)
{
	return (
		((QTUtils_GetQTVersion() >> 16) & 0xffff) >= kQTVideoEffectsMinVers);
}

//////////
//
// QTUtils_HasFullScreenSupport
// Does the installed version of QuickTime support the full-screen routines?
//
// Note: this is a pretty cheesy way of determining whether full-screen routines
// are available.
//
//////////

Boolean QTUtils_HasFullScreenSupport(void)
{
	return (((QTUtils_GetQTVersion() >> 16) & 0xffff) >= kQTFullScreenMinVers);
}

//////////
//
// QTUtils_HasWiredSprites
// Does the installed version of QuickTime support wired sprites?
//
// Note: this is a pretty cheesy way of determining whether wired sprites are
// available.
//
//////////

Boolean QTUtils_HasWiredSprites(void)
{
	return (
		((QTUtils_GetQTVersion() >> 16) & 0xffff) >= kQTWiredSpritesMinVers);
}

//////////
//
// QTUtils_IsQTVRMovie
// Is the specified movie a QTVR movie?
//
// WARNING: This function is intended for use ONLY when you want to determine if
// you've got a QTVR movie but you don't want to use the QuickTime VR API
// (perhaps QTVR isn't installed...). The preferred way to determine if a movie
// is a QTVR movie is to call QTVRGetQTVRTrack and then QTVRGetQTVRInstance; if
// you get back a non-NULL instance, you've got a QTVR movie.
//
//////////

Boolean QTUtils_IsQTVRMovie(Movie theMovie)
{
	Boolean myIsQTVRMovie = false;
	OSType myType;

	// QTVR movies have a special piece of user data identifying the movie
	// controller type
	myType = QTUtils_GetControllerType(theMovie);

	if ((myType == kQTVRQTVRType) || (myType == kQTVROldPanoType) ||
		(myType == kQTVROldObjectType))
		myIsQTVRMovie = true;

	return (myIsQTVRMovie);
}

//////////
//
// QTUtils_IsStreamedMovie
// Is the specified movie a streamed movie?
//
//////////

Boolean QTUtils_IsStreamedMovie(Movie theMovie)
{
	return (GetMovieIndTrackType(theMovie, 1, kQTSStreamMediaType,
				movieTrackMediaType | movieTrackEnabledOnly) != NULL);
}

//////////
//
// QTUtils_IsAutoPlayMovie
// Get the autoplay state of a movie file.
//
// A movie file can have information about its autoplay state in a user data
// item of type 'play'. If the movie doesn't contain an item of this type, then
// we'll assume that it doesn't autoplay. If it does contain an item of this
// type, then the item data (a Boolean) is 0 for normal play and 1 for autoplay.
//
//////////

Boolean QTUtils_IsAutoPlayMovie(Movie theMovie)
{
	UserData myUserData = NULL;
	Boolean myAutoPlay = false;
	OSErr myErr = paramErr;

	// make sure we've got a movie
	if (theMovie == NULL)
		return (myAutoPlay);

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData != NULL) {
		myErr = GetUserDataItem(myUserData, &myAutoPlay, sizeof(myAutoPlay),
			FOUR_CHAR_CODE('play'), 0);
		if (myErr != noErr)
			myAutoPlay = false;
	}

	return (myAutoPlay);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Controller bar utilities.
//
// Use these functions to manipulate the controller bar, its buttons, and the
// help text displayed in it.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////
//
// QTUtils_IsControllerBarVisible
// Is the controller bar currently visible?
//
//////////

Boolean QTUtils_IsControllerBarVisible(MovieController theMC)
{
	return ((Boolean)MCGetVisible(theMC));
}

//////////
//
// QTUtils_GetControllerBarHeight
// Return the height of the controller bar displayed by the movie controller.
//
// Note that MCGetControllerBoundsRect returns the rectangle of bar and movie,
// if attached; so we need to detach the controller bar first.
//
//////////

short QTUtils_GetControllerBarHeight(MovieController theMC)
{
	Boolean wasAttached = false;
	Rect myRect;

	// if the controller bar is attached, detach it (and remember we did so)
	if (MCIsControllerAttached(theMC) == 1) {
		wasAttached = true;
		MCSetControllerAttached(theMC, false);
	}

	// get the rectangle of the controller
	MCGetControllerBoundsRect(theMC, &myRect);

	// now reattach the controller bar, if it was originally attached
	if (wasAttached)
		MCSetControllerAttached(theMC, true);

	return (short)(myRect.bottom - myRect.top);
}

//////////
//
// QTUtils_HideControllerBar
// Hide the controller bar provided by the movie controller.
//
//////////

void QTUtils_HideControllerBar(MovieController theMC)
{
	MCSetVisible(theMC, false);
}

//////////
//
// QTUtils_ShowControllerBar
// Show the controller bar provided by the movie controller.
//
//////////

void QTUtils_ShowControllerBar(MovieController theMC)
{
	MCSetVisible(theMC, true);
}

//////////
//
// QTUtils_ToggleControllerBar
// Toggle the state of the controller bar provided by the movie controller.
//
//////////

void QTUtils_ToggleControllerBar(MovieController theMC)
{
	if (QTUtils_IsControllerBarVisible(theMC))
		QTUtils_HideControllerBar(theMC);
	else
		QTUtils_ShowControllerBar(theMC);
}

//////////
//
// QTUtils_PutControllerBarOnTop
// Put the movie controller bar at the top of the movie window.
//
//////////

void QTUtils_PutControllerBarOnTop(MovieController theMC)
{
	if (theMC == NULL)
		return;

	if (MCIsControllerAttached(theMC) == 1) {
		Rect myMCRect;
		Rect myMovieRect;

		MCGetControllerBoundsRect(theMC, &myMCRect);
		myMovieRect = myMCRect;
		myMCRect.bottom =
			(short)(myMCRect.top + QTUtils_GetControllerBarHeight(theMC));
		myMovieRect.top = (short)(myMCRect.bottom + 1);

		MCSetControllerAttached(theMC, false);

		MCPositionController(theMC, &myMovieRect, &myMCRect, 0L);
	}
}

//////////
//
// QTUtils_HideControllerButton
// Hide the specified button in the controller bar.
//
// Some explanation is probably useful here: the first thing to understand is
// that every VR movie has TWO sets of movie controller flags: (1) a set of
// "control flags" and (2) a set of "explicit flags".
//
// The control flags work as documented in IM: QuickTime Components (pp. 2-20f)
// and in VRPWQTVR2.0 (pp. 2-23f): if a bit in the set of control flags is set
// (that is, equal to 1), then the associated action or property is enabled. For
// instance, bit 17 (mcFlagQTVRSuppressZoomBtns) means to suppress the zoom
// buttons. So, if that bit is set in a VR movie's control flags, the zoom
// buttons are NOT displayed. If that bit is clear, the zoom buttons are
// displayed.
//
// However, the QuickTime VR movie controller sometimes suppresses buttons even
// when those buttons have not been explicitly suppressed in the control flags.
// For example, if a particular VR movie does not contain a sound track, then
// the movie controller automatically suppresses the speaker/volume button.
// Likewise, if a movie does contain a sound track, then the speaker/volume
// button is automatically displayed, again without regard to the actual value
// of bit 2 in the control flags.
//
// This might not be what you'd like to happen. For instance, if your
// application is playing a sound that it loaded from a sound resource, you
// might want the user to be able to adjust the sound's volume using the volume
// control. To do that, you need a way to *force* the speaker/volume button to
// appear. For this reason, the explicit flags were introduced.
//
// The explicit flags indicate which bits in the control flags are to be used
// explicitly (that is, taken at face value). If a certain bit is set in a
// movie's explicit flags, then the corresponding bit in the control flags is
// interpreted as the desired setting for the feature (and the movie controller
// will not attempt to do anything "clever"). In other words, if bit 17 is set
// in a movie's explicit flags and bit 17 is clear in that movie's control
// flags, then the zoom buttons are displayed. Similarly, if bit 2 is set in a
// movie's explicit flags and bit 2 is clear in that movie's control flags, then
// the speaker/volume button is displayed, whether or not the movie contains a
// sound track.
//
// The final thing to understand: to get or set a bit in a movie's explicit
// flags, you must set the flag mcFlagQTVRExplicitFlagSet in your call to
// mcActionGetFlags or mcActionSetFlags. To get or set a bit in a movie's
// control flags, you must clear the flag mcFlagQTVRExplicitFlagSet in your call
// to mcActionGetFlags or mcActionSetFlags. Note that when you use the defined
// constants to set values in the explicit flags, the constant names might be
// confusing. For instance, setting the bit mcFlagSuppressSpeakerButton in a
// movie's explicit flags doesn't cause the speaker to be suppressed; it just
// means: "use the actual value of the mcFlagSuppressSpeakerButton bit in the
// control flags".
//
// Whew! Any questions? Okay, then now you'll understand how to hide or show a
// button in the controller bar: set the appropriate explicit flag to 1 and set
// the corresponding control flag to the desired value. And you'll understand
// how to let the movie controller do its "clever" work: clear the appropriate
// explicit flag.
//
// There is one final twist to this story: the mcFlag bit for the custom
// controller button indicates that the custom button should be shown, not (like
// all other button flags) that the button should be suppressed. So we need to
// treat the custom button specially. Sigh.
//
//////////

void QTUtils_HideControllerButton(MovieController theMC, long theButton)
{
	long myControllerFlags;

	// handle the custom button separately
	if (theButton == kQTUtilsCustomButton) {
		MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
		MCDoAction(
			theMC, mcActionSetFlags, (void*)(myControllerFlags & ~theButton));
	} else {
		// get the current explicit flags and set the explicit flag for the
		// specified button
		myControllerFlags = mcFlagQTVRExplicitFlagSet;
		MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
		MCDoAction(theMC, mcActionSetFlags,
			(void*)((myControllerFlags | theButton) |
				mcFlagQTVRExplicitFlagSet));

		// get the current control flags and set the suppress flag for the
		// specified button
		myControllerFlags = 0;
		MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
		MCDoAction(theMC, mcActionSetFlags,
			(void*)((myControllerFlags | theButton) &
				~mcFlagQTVRExplicitFlagSet));
	}
}

//////////
//
// QTUtils_ShowControllerButton
// Show the specified button in the controller bar.
//
//////////

void QTUtils_ShowControllerButton(MovieController theMC, long theButton)
{
	long myControllerFlags;

	// handle the custom button separately
	if (theButton == kQTUtilsCustomButton) {
		MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
		MCDoAction(
			theMC, mcActionSetFlags, (void*)(myControllerFlags | theButton));
	} else {
		// get the current explicit flags and set the explicit flag for the
		// specified button
		myControllerFlags = mcFlagQTVRExplicitFlagSet;
		MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
		MCDoAction(theMC, mcActionSetFlags,
			(void*)((myControllerFlags | theButton) |
				mcFlagQTVRExplicitFlagSet));

		// get the current control flags and clear the suppress flag for the
		// specified button
		myControllerFlags = 0;
		MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
		MCDoAction(theMC, mcActionSetFlags,
			(void*)(myControllerFlags & ~theButton &
				~mcFlagQTVRExplicitFlagSet));
	}
}

//////////
//
// QTUtils_ToggleControllerButton
// Toggle the state of the specified button in the controller bar.
//
//////////

void QTUtils_ToggleControllerButton(MovieController theMC, long theButton)
{
	if (QTUtils_IsControllerButtonVisible(theMC, theButton))
		QTUtils_HideControllerButton(theMC, theButton);
	else
		QTUtils_ShowControllerButton(theMC, theButton);
}

//////////
//
// QTUtils_ResetControllerButton
// Remove any explicit setting of the specified button in the controller bar.
// (This allows the QuickTime VR movie controller to be as clever as it knows
// how to be.)
//
//////////

void QTUtils_ResetControllerButton(MovieController theMC, long theButton)
{
	long myControllerFlags = mcFlagQTVRExplicitFlagSet;

	// get the current explicit flags and clear the explicit flag for the
	// specified button
	MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);
	MCDoAction(theMC, mcActionSetFlags,
		(void*)((myControllerFlags | theButton) & ~mcFlagQTVRExplicitFlagSet));
}

//////////
//
// QTUtils_IsControllerButtonVisible
// Is the specified button in the controller bar currently visible?
//
//////////

Boolean QTUtils_IsControllerButtonVisible(MovieController theMC, long theButton)
{
	long myControllerFlags;

	// get the current control flags
	myControllerFlags = 0;
	MCDoAction(theMC, mcActionGetFlags, &myControllerFlags);

	// the speaker button requires some additional logic, because the QTVR movie
	// controller treats it special; be advised that that controller's special
	// behavior could change in the future, so you might need to tweak this code
	if (theButton == mcFlagSuppressSpeakerButton) {
		long myExplicitFlags;

		// get the current explicit flags
		myExplicitFlags = mcFlagQTVRExplicitFlagSet;
		MCDoAction(theMC, mcActionGetFlags, &myExplicitFlags);

		// the speaker button is not showing if the movie has no sound track and
		// the explicit flag is not set
		if (!QTUtils_MovieHasSoundTrack(MCGetMovie(theMC)) &&
			!(myExplicitFlags & theButton))
			return (false);
	}

	// the custom button requires some different treatment, since it doesn't
	// have a "Suppress" button constant
	if (theButton == mcFlagsUseCustomButton)
		if (myControllerFlags & theButton)
			return (true);
		else
			return (false);

	// examine the suppress flag for the specified button
	if (myControllerFlags &
		theButton) // if the button is currently suppressed...
		return (false);
	else
		return (true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Media utilities.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////
//
// QTUtils_IsMediaTypeInMovie
// Determine whether a specific media type is in a movie.
//
//////////

Boolean QTUtils_IsMediaTypeInMovie(Movie theMovie, OSType theMediaType)
{
	return (GetMovieIndTrackType(theMovie, 1, theMediaType,
				movieTrackMediaType | movieTrackEnabledOnly) != NULL);
}

//////////
//
// QTUtils_MovieHasTimeCodeTrack
// Determine whether a movie contains a timecode track.
//
//////////

Boolean QTUtils_MovieHasTimeCodeTrack(Movie theMovie)
{
	return (GetMovieIndTrackType(
				theMovie, 1, TimeCodeMediaType, movieTrackMediaType) != NULL);
}

//////////
//
// QTUtils_MovieHasSoundTrack
// Determine whether a movie contains a sound track.
//
//////////

Boolean QTUtils_MovieHasSoundTrack(Movie theMovie)
{
	return (GetMovieIndTrackType(theMovie, 1, AudioMediaCharacteristic,
				movieTrackCharacteristic | movieTrackEnabledOnly) != NULL);
}

//////////
//
// QTUtils_GetSoundMediaHandler
// Return the sound media handler for a movie.
//
//////////

MediaHandler QTUtils_GetSoundMediaHandler(Movie theMovie)
{
	Track myTrack;
	Media myMedia;

	myTrack = GetMovieIndTrackType(theMovie, 1, AudioMediaCharacteristic,
		movieTrackCharacteristic | movieTrackEnabledOnly);
	if (myTrack != NULL) {
		myMedia = GetTrackMedia(myTrack);
		return (GetMediaHandler(myMedia));
	}

	return (NULL);
}

//////////
//
// QTUtils_UpdateMovieVolumeSetting
// Update the preferred volume setting of the specified movie.
//
//////////

OSErr QTUtils_UpdateMovieVolumeSetting(Movie theMovie)
{
	short myPrefVolume;
	short myCurrVolume;
	OSErr myErr = noErr;

	myPrefVolume = GetMoviePreferredVolume(theMovie);
	myCurrVolume = GetMovieVolume(theMovie);
	myCurrVolume = (short)abs(myCurrVolume);

	if (myPrefVolume != myCurrVolume) {
		SetMoviePreferredVolume(theMovie, myCurrVolume);
		myErr = GetMoviesError();
	}

	return (myErr);
}

//////////
//
// QTUtils_SelectAllMovie
// Select the entire movie associated with the specified movie controller.
//
//////////

OSErr QTUtils_SelectAllMovie(MovieController theMC)
{
	TimeRecord myTimeRecord;
	Movie myMovie = NULL;
	ComponentResult myErr = noErr;

	if (theMC == NULL)
		return (paramErr);

	myMovie = MCGetMovie(theMC);
	if (myMovie == NULL)
		return (paramErr);

	myTimeRecord.value.hi = 0;
	myTimeRecord.value.lo = 0;
	myTimeRecord.base = 0;
	myTimeRecord.scale = GetMovieTimeScale(myMovie);
	myErr = MCDoAction(theMC, mcActionSetSelectionBegin, &myTimeRecord);
	if (myErr != noErr)
		return ((OSErr)myErr);

	myTimeRecord.value.hi = 0;
	myTimeRecord.value.lo = (UInt32)GetMovieDuration(myMovie);
	myTimeRecord.base = 0;
	myTimeRecord.scale = GetMovieTimeScale(myMovie);
	myErr = MCDoAction(theMC, mcActionSetSelectionDuration, &myTimeRecord);

	return ((OSErr)myErr);
}

//////////
//
// QTUtils_SelectNoneMovie
// Select none of the movie associated with the specified movie controller.
//
//////////

OSErr QTUtils_SelectNoneMovie(MovieController theMC)
{
	TimeRecord myTimeRecord;
	Movie myMovie = NULL;
	ComponentResult myErr = noErr;

	if (theMC == NULL)
		return (paramErr);

	myMovie = MCGetMovie(theMC);
	if (myMovie == NULL)
		return (paramErr);

	myTimeRecord.value.hi = 0;
	myTimeRecord.value.lo = 0;
	myTimeRecord.base = 0;
	myTimeRecord.scale = GetMovieTimeScale(myMovie);
	myErr = MCDoAction(theMC, mcActionSetSelectionDuration, &myTimeRecord);

	return ((OSErr)myErr);
}

//////////
//
// QTUtils_MakeSampleDescription
// Return a new image description with default and specified values.
//
//////////

ImageDescriptionHandle QTUtils_MakeSampleDescription(
	long theEffectType, short theWidth, short theHeight)
{
	ImageDescriptionHandle mySampleDesc = NULL;

	// create a new sample description
	mySampleDesc =
		(ImageDescriptionHandle)NewHandleClear(sizeof(ImageDescription));
	if (mySampleDesc == NULL)
		return (NULL);

	// fill in the fields of the sample description
	(**mySampleDesc).idSize = sizeof(ImageDescription);
	(**mySampleDesc).cType = (CodecType)theEffectType;
	(**mySampleDesc).vendor = kAppleManufacturer;
	(**mySampleDesc).temporalQuality = codecNormalQuality;
	(**mySampleDesc).spatialQuality = codecNormalQuality;
	(**mySampleDesc).width = theWidth;
	(**mySampleDesc).height = theHeight;
	(**mySampleDesc).hRes = 72L << 16;
	(**mySampleDesc).vRes = 72L << 16;
	(**mySampleDesc).dataSize = 0L;
	(**mySampleDesc).frameCount = 1;
	(**mySampleDesc).depth = 24;
	(**mySampleDesc).clutID = -1;

	return (mySampleDesc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// User data utilities.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#if CONTENT_RATING_AVAIL
//////////
//
// QTUtils_GetContentRatingFromMovie
// Get the content rating from a movie; return an error if it has none. In any
// case, return a meaningful content rating.
//
//////////

OSErr QTUtils_GetContentRatingFromMovie(
	Movie theMovie, UInt16* theRating, UInt32* theReasons)
{
	UserData myUserData = NULL;
	QTAltContentRatingRecord myContentRec;
	OSErr myErr = paramErr;

	*theRating = kQTContentTVYRating;
	*theReasons = 0L;

	// make sure we've got a movie
	if (theMovie == NULL)
		return (myErr);

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData != NULL) {
		myErr = GetUserDataItem(myUserData, &myContentRec, sizeof(myContentRec),
			FOUR_CHAR_CODE('crat'), 0);
		if (myErr == noErr) {
			*theRating = EndianU16_BtoN(myContentRec.ratings);
			*theReasons = EndianU32_BtoN(myContentRec.contentType);
		}
	}

	return (myErr);
}

//////////
//
// QTUtils_AddContentRatingToMovie
// Add a content rating to a movie.
//
// A content rating is stored as a user data item that indicates both the
// general rating (for example, TV-MA [mature audiences only]) and any
// additional information about the content (for example, coarse language).
// Let's call this additional information the "reasons" for the rating. Both the
// rating and the reasons are specified using constants in the file
// MoviesFormat.h.
//
// This function adds the specified content rating to the movie's user data;
// the updated user data is written to the movie file when the movie is next
// updated (by calling UpdateMovieResource).
//
//////////

OSErr QTUtils_AddContentRatingToMovie(
	Movie theMovie, UInt16 theRating, UInt32 theReasons)
{
	UserData myUserData = NULL;
	QTAltContentRatingRecord myContentRec;
	OSErr myErr = noErr;

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData == NULL)
		return (paramErr);

	myContentRec.flags = 0;
	myContentRec.contentType = EndianU32_NtoB(theReasons);
	myContentRec.ratings = EndianU16_NtoB(theRating);

	// for simplicity, we assume that we want only one content rating in the
	// movie; as a result, we won't worry about overwriting any existing item of
	// that type

	// add the data to the movie's user data
	myErr = SetUserDataItem(myUserData, &myContentRec, sizeof(myContentRec),
		FOUR_CHAR_CODE('crat'), 0);

	return (myErr);
}
#endif // #if CONTENT_RATING_AVAIL

//////////
//
// QTUtils_AddUserDataTextToMovie
// Add a user data item, of the specified type, containing the specified text to
// a movie.
//
// This function adds the specified text to the movie's user data;
// the updated user data is written to the movie file when the movie is next
// updated (by calling UpdateMovieResource).
//
//////////

OSErr QTUtils_AddUserDataTextToMovie(
	Movie theMovie, char* theText, OSType theType)
{
	UserData myUserData = NULL;
	Handle myHandle = NULL;
	long myLength = (long)strlen(theText);
	OSErr myErr = noErr;

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData == NULL)
		return (paramErr);

	// copy the specified text into a new handle
	myHandle = NewHandleClear(myLength);
	if (myHandle == NULL)
		return (MemError());

	BlockMoveData(theText, *myHandle, myLength);

	// for simplicity, we assume that we want only one user data item of the
	// specified type in the movie; as a result, we won't worry about
	// overwriting any existing item of that type....
	//
	// if you need multiple user data items of a given type (for example, a
	// copyright notice in several different languages), you would need to
	// modify this code; this is left as an exercise for the reader....

	// add the data to the movie's user data
	myErr = AddUserDataText(myUserData, myHandle, theType, 1,
		(short)GetScriptManagerVariable(smRegionCode));

	// clean up
	DisposeHandle(myHandle);
	return (myErr);
}

//////////
//
// QTUtils_AddCopyrightToMovie
// Add a user data item containing the specified copyright text to a movie.
//
//////////

OSErr QTUtils_AddCopyrightToMovie(Movie theMovie, char* theText)
{
	return QTUtils_AddUserDataTextToMovie(
		theMovie, theText, (OSType)kUserDataTextCopyright);
}

//////////
//
// QTUtils_AddMovieNameToMovie
// Add a user data item containing the specified name to a movie.
//
//////////

OSErr QTUtils_AddMovieNameToMovie(Movie theMovie, char* theText)
{
	return QTUtils_AddUserDataTextToMovie(
		theMovie, theText, (OSType)kUserDataTextFullName);
}

//////////
//
// QTUtils_AddMovieInfoToMovie
// Add a user data item containing the specified information to a movie.
//
//////////

OSErr QTUtils_AddMovieInfoToMovie(Movie theMovie, char* theText)
{
	return QTUtils_AddUserDataTextToMovie(
		theMovie, theText, (OSType)kUserDataTextInformation);
}

//////////
//
// QTUtils_GetMovieFileLoopingInfo
// Get the looping state of a movie file.
//
// A movie file can have information about its looping state in a user data item
// of type 'LOOP'. If the movie doesn't contain an item of this type, then we'll
// assume that it isn't looping. If it does contain an item of this type, then
// the item data (a long integer) is 0 for normal looping and 1 for palindrome
// looping. Accordingly, this function returns the following values in the
// theLoopInfo parameter:
//
//		0 == normal looping
//		1 == palindrome looping
//		2 == no looping
//
// Return an error if the movie has no looping state. In any case, return a
// meaningful looping state.
//
//////////

OSErr QTUtils_GetMovieFileLoopingInfo(Movie theMovie, long* theLoopInfo)
{
	UserData myUserData = NULL;
	long myLoopInfo = kNoLooping;
	OSErr myErr = paramErr;

	// make sure we've got a movie
	if (theMovie == NULL)
		goto bail;

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData != NULL) {
		myErr = GetUserDataItem(myUserData, &myLoopInfo, sizeof(myLoopInfo),
			FOUR_CHAR_CODE('LOOP'), 0);
		if (myErr == noErr)
			myLoopInfo = EndianS32_BtoN(myLoopInfo);
	}

bail:
	*theLoopInfo = myLoopInfo;

	return (myErr);
}

//////////
//
// QTUtils_SetMovieFileLoopingInfo
// Set the looping state for a movie file.
//
//////////

OSErr QTUtils_SetMovieFileLoopingInfo(Movie theMovie, long theLoopInfo)
{
	UserData myUserData = NULL;
	long myLoopInfo;
	short myCount = 0;
	OSErr myErr = paramErr;

	// get the movie's user data
	myUserData = GetMovieUserData(theMovie);
	if (myUserData == NULL)
		goto bail;

	// we want to end up with at most one user data item of type 'LOOP',
	// so let's remove any existing ones
	myCount = CountUserDataType(myUserData, FOUR_CHAR_CODE('LOOP'));
	while (myCount--)
		RemoveUserData(myUserData, FOUR_CHAR_CODE('LOOP'), 1);

	// make sure we're writing big-endian data
	myLoopInfo = (long)EndianU32_NtoB(theLoopInfo);

	switch (theLoopInfo) {
	case kNormalLooping:
	case kPalindromeLooping:
		myErr = SetUserDataItem(
			myUserData, &myLoopInfo, sizeof(long), FOUR_CHAR_CODE('LOOP'), 0);
		break;

	case kNoLooping:
	default:
		myErr = noErr;
		break;
	}

bail:
	return (myErr);
}

//////////
//
// QTUtils_SetLoopingStateFromFile
// Set the looping state for a movie based on the looping information in the
// movie file.
//
//////////

OSErr QTUtils_SetLoopingStateFromFile(Movie theMovie, MovieController theMC)
{
	long myLoopInfo = kNoLooping;
	OSErr myErr = noErr;

	myErr = QTUtils_GetMovieFileLoopingInfo(theMovie, &myLoopInfo);
	switch (myLoopInfo) {

	case kNormalLooping:
		MCDoAction(theMC, mcActionSetLooping, (void*)true);
		MCDoAction(theMC, mcActionSetLoopIsPalindrome, (void*)false);
		break;

	case kPalindromeLooping:
		MCDoAction(theMC, mcActionSetLooping, (void*)true);
		MCDoAction(theMC, mcActionSetLoopIsPalindrome, (void*)true);
		break;

	case kNoLooping:
	default:
		MCDoAction(theMC, mcActionSetLooping, (void*)false);
		MCDoAction(theMC, mcActionSetLoopIsPalindrome, (void*)false);
		break;
	}

	return (myErr);
}

//////////
//
// QTUtils_MakeMovieLoop
// Set the specified movie to loop.
//
// Based on the function MakeMovieLoop by Kevin Marks.
//
//////////

OSErr QTUtils_MakeMovieLoop(Movie theMovie, Boolean isPalindrome)
{
	TimeBase myTimeBase = NULL;
	long myFlags = 0L;
	OSErr myErr = paramErr;

	// make sure we've got a movie
	if (theMovie == NULL)
		goto bail;

	myErr = noErr;

	// set the movie's play hints to enhance looping performance
	SetMoviePlayHints(theMovie, hintsLoop, hintsLoop);

	// set the looping flag of the movie's time base
	myTimeBase = GetMovieTimeBase(theMovie);
	myFlags = GetTimeBaseFlags(myTimeBase);
	myFlags |= loopTimeBase;

	// set or clear the palindrome flag, depending on the specified setting
	if (isPalindrome)
		myFlags |= palindromeLoopTimeBase;
	else
		myFlags &= ~palindromeLoopTimeBase;

	SetTimeBaseFlags(myTimeBase, myFlags);

bail:
	return (myErr);
}

//////////
//
// QTUtils_GetWindowPositionFromFile
// Return, through thePoint, the stored position of the specified movie.
//
// Return an error if the movie has no stored position. In any case, return a
// meaningful position.
//
//////////

OSErr QTUtils_GetWindowPositionFromFile(Movie theMovie, Point* thePoint)
{
	UserData myUserData = NULL;
	Point myPoint = {kDefaultWindowX, kDefaultWindowY};
	OSErr myErr = paramErr;

	// make sure we've got a movie
	if (theMovie == NULL)
		goto bail;

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData != NULL) {
		myErr = GetUserDataItem(
			myUserData, &myPoint, sizeof(Point), FOUR_CHAR_CODE('WLOC'), 0);
		if (myErr == noErr) {
			myPoint.v = EndianS16_BtoN(myPoint.v);
			myPoint.h = EndianS16_BtoN(myPoint.h);
		}
	}

bail:
	if (thePoint != NULL)
		*thePoint = myPoint;

	return (myErr);
}

//////////
//
// QTUtils_GetTrackName
// Get the name of the specified movie track.
//
// This routine is modelled on the one contained in Dispatch 2 from the Ice
// Floe; I've modified it to return a C string instead of a Pascal string.
//
// The caller is responsible for disposing of the pointer returned by this
// function (by calling free).
//
//////////

char* QTUtils_GetTrackName(Track theTrack)
{
	UserData myUserData = NULL;
	char* myString = NULL;
	OSErr myErr = noErr;

	// make sure we've got a track
	if (theTrack == NULL)
		return (NULL);

	// a track's name (if it has one) is stored in the track's user data
	myUserData = GetTrackUserData(theTrack);
	if (myUserData != NULL) {
		Handle myHandle = NewHandle(0);

		// get the user data item of type kUserDataName;
		// the handle we pass to GetUserData is resized to contain the track
		// name
		myErr = GetUserData(myUserData, myHandle, kUserDataName, 1);
		if (myErr == noErr) {
			long myLength = GetHandleSize(myHandle);

			if (myLength > 0) {
				myString = malloc((size_t)(myLength + 1));
				if (myString != NULL) {
					memcpy(myString, *myHandle, (size_t)myLength);
					myString[myLength] = '\0';
				}
			}
		}

		DisposeHandle(myHandle);
	}

	return (myString);
}

//////////
//
// QTUtils_SetTrackName
// Set the name of the specified movie track.
//
// This function adds the specified text to the track's user data;
// the updated user data is written to the movie file when the movie is next
// updated (by calling UpdateMovieResource).
//
//////////

OSErr QTUtils_SetTrackName(Track theTrack, char* theText)
{
	UserData myUserData = NULL;
	OSErr myErr = noErr;

	// make sure we've got a track and a name
	if ((theTrack == NULL) || (theText == NULL))
		return (paramErr);

	// get the track's user data list
	myUserData = GetTrackUserData(theTrack);
	if (myUserData == NULL)
		return (paramErr);

	// remove any existing track name
	while (RemoveUserData(myUserData, kUserDataName, 1) == noErr)
		;

	myErr = SetUserDataItem(
		myUserData, theText, (long)strlen(theText), kUserDataName, 0);

	return (myErr);
}

//////////
//
// QTUtils_MakeTrackNameByType
// Create a (unique) name for the specified track, based on the track's type.
//
// Given a movie track, this routine constructs a name for that track based on
// the media type of that track. For instance, if the track is a sound track,
// this routine returns the name "Sound". However, if there is more than one
// track of that media type, then this routine numbers the track names. So, if
// there are two sound tracks, this routine names them "Sound 1" and "Sound 2".
//
// This routine is modelled on the one contained in Dispatch 2 from the Ice
// Floe; I've modified it to return a C string instead of a Pascal string.
//
// The caller is responsible for disposing of the pointer returned by this
// function (by calling free).
//
//////////

char* QTUtils_MakeTrackNameByType(Track theTrack)
{
	Media myMedia;
	char* myString = NULL;
	ComponentResult myErr = noErr;

	// make sure we've got a track
	if (theTrack == NULL)
		return (NULL);

	myMedia = GetTrackMedia(theTrack);
	if (myMedia != NULL) {
		MediaHandler myMediaHandler = GetMediaHandler(myMedia);
		OSType myMediaType;
		Str255 myName;

		// get the media type of the track
		GetMediaHandlerDescription(myMedia, &myMediaType, NULL, NULL);

		// get the text of the media type
		myErr = MediaGetName(myMediaHandler, myName, 0, NULL);
		if (myErr == noErr) {

			// determine whether there's more than one track with this media
			// type
			if (GetMovieIndTrackType(GetTrackMovie(theTrack), 2, myMediaType,
					movieTrackMediaType) != NULL) {

				// add an index number to the track type string we constructed
				// above
				long myIndex = 1;
				Str255 myNumString;

				while (GetMovieIndTrackType(GetTrackMovie(theTrack), myIndex,
						   myMediaType, movieTrackMediaType) != theTrack)
					myIndex++;

				NumToString(myIndex, myNumString);
				myName[++myName[0]] = ' ';
				BlockMoveData(
					&myNumString[1], &myName[myName[0] + 1], myNumString[0]);
				myName[0] += myNumString[0];
			}

			// now copy the string data from the Pascal string to a C string
			if (myName[0] > 0) {
				myString = malloc((size_t)(myName[0] + 1));
				if (myString != NULL) {
					memcpy(myString, &myName[1], myName[0]);
					myString[myName[0]] = '\0';
				}
			}
		}
	}

	return (myString);
}

//////////
//
// QTUtils_IsImageFile
// Is the specified file an image file?
//
//////////

Boolean QTUtils_IsImageFile(FSSpec* theFSSpec)
{
	GraphicsImportComponent myImporter = NULL;

	GetGraphicsImporterForFile(theFSSpec, &myImporter);
	if (myImporter != NULL)
		CloseComponent(myImporter);

	return (myImporter != NULL);
}

//////////
//
// QTUtils_IsMovieFile
// Is the specified file a file that can be opened by QuickTime as a movie?
//
//////////

Boolean QTUtils_IsMovieFile(FSSpec* theFSSpec)
{
	Boolean isMovieFile = false;
	AliasHandle myAlias = NULL;
	Component myImporter = NULL;
	FInfo myFinderInfo;
	OSErr myErr = noErr;

	// see whether the file type is MovieFileType; to do this, get the Finder
	// information
	myErr = FSpGetFInfo(theFSSpec, &myFinderInfo);
	if (myErr == noErr)
		if (myFinderInfo.fdType == kQTFileTypeMovie)
			return (true);

	// if it isn't a movie file, see whether the file can be imported as a movie
	myErr = QTNewAlias(theFSSpec, &myAlias, true);
	if (myErr == noErr) {
		if (myAlias != NULL) {
			myErr = GetMovieImporterForDataRef(rAliasType, (Handle)myAlias,
				kGetMovieImporterDontConsiderGraphicsImporters, &myImporter);
			DisposeHandle((Handle)myAlias);
		}
	}

	if ((myErr == noErr) && (myImporter != NULL)) // this file is a movie file
		isMovieFile = true;

	return (isMovieFile);
}

//////////
//
// QTUtils_ConvertFloatToBigEndian
// Convert the specified floating-point number to big-endian format.
//
//////////

void QTUtils_ConvertFloatToBigEndian(float* theFloat)
{
	unsigned long* myLongPtr;

	myLongPtr = (unsigned long*)theFloat;
	*myLongPtr = EndianU32_NtoB(*myLongPtr);
}

//////////
//
// QTUtils_ConvertCToPascalString
// Convert a C string into a Pascal string.
//
// The caller is responsible for disposing of the pointer returned by this
// function (by calling free).
//
//////////

StringPtr QTUtils_ConvertCToPascalString(char* theString)
{
	StringPtr myString = malloc(min(strlen(theString) + 1, 256));
	short myIndex = 0;

	while ((theString[myIndex] != '\0') && (myIndex < 255)) {
		myString[myIndex + 1] = (unsigned char)theString[myIndex];
		myIndex++;
	}

	myString[0] = (unsigned char)myIndex;

	return (myString);
}

//////////
//
// QTUtils_ConvertPascalToCString
// Convert a Pascal string into a C string.
//
// The caller is responsible for disposing of the pointer returned by this
// function (by calling free).
//
//////////

char* QTUtils_ConvertPascalToCString(StringPtr theString)
{
	char* myString = malloc((size_t)(theString[0] + 1));
	short myIndex = 0;

	for (myIndex = 0; myIndex < theString[0]; myIndex++)
		myString[myIndex] = (char)theString[myIndex + 1];

	myString[theString[0]] = '\0';

	return (myString);
}

//////////
//
// QTUtils_DeleteAllReferencesToTrack
// Delete all existing track references to the specified track.
//
//////////

OSErr QTUtils_DeleteAllReferencesToTrack(Track theTrack)
{
	Movie myMovie = NULL;
	Track myTrack = NULL;
	long myTrackCount = 0L;
	long myTrRefCount = 0L;
	long myTrackIndex;
	long myTrRefIndex;
	OSErr myErr = noErr;

	myMovie = GetTrackMovie(theTrack);
	if (myMovie == NULL)
		return (paramErr);

	// iterate thru all the tracks in the movie (that are different from the
	// specified track)
	myTrackCount = GetMovieTrackCount(myMovie);
	for (myTrackIndex = 1; myTrackIndex <= myTrackCount; myTrackIndex++) {
		myTrack = GetMovieIndTrack(myMovie, myTrackIndex);
		if ((myTrack != NULL) && (myTrack != theTrack)) {
			OSType myType = 0L;

			// iterate thru all track reference types contained in the current
			// track
			myType = GetNextTrackReferenceType(myTrack, myType);
			while (myType != 0L) {

				// iterate thru all track references of the current type;
				// note that we count down to 1, since DeleteTrackReference will
				// cause any higher-indexed track references to be renumbered
				myTrRefCount = GetTrackReferenceCount(myTrack, myType);
				for (myTrRefIndex = myTrRefCount; myTrRefIndex >= 1;
					 myTrRefIndex--) {
					Track myRefTrack = NULL;

					myRefTrack =
						GetTrackReference(myTrack, myType, myTrRefIndex);
					if (myRefTrack == theTrack)
						myErr =
							DeleteTrackReference(myTrack, myType, myTrRefIndex);
				}

				myType = GetNextTrackReferenceType(myTrack, myType);
			}
		}
	}

	return (myErr);
}

//////////
//
// QTUtils_GetFrameDuration
// Get the duration of the first sample frame in the specified movie track.
//
//////////

TimeValue QTUtils_GetFrameDuration(Track theTrack)
{
	TimeValue mySampleDuration;
	OSErr myErr = noErr;

	myErr = GetMediaSample(GetTrackMedia(theTrack),
		NULL, // don't return sample data
		0,
		NULL, // don't return number of bytes of sample data returned
		0, NULL, &mySampleDuration,
		NULL,  // don't return sample description
		NULL,  // don't return sample description index
		0,     // no max number of samples
		NULL,  // don't return number of samples returned
		NULL); // don't return sample flags

	// make sure we return a legitimate value even if GetMediaSample encounters
	// an error
	if (myErr != noErr)
		mySampleDuration = 0;

	return (mySampleDuration);
}

//////////
//
// QTUtils_GetFrameCount
// Get the number of frames in the specified movie track. We return the value -1
// if an error occurs and we cannot determine the number of frames in the track.
//
// Based (loosely) on frame-counting code in ConvertToMovie Jr.c.
//
// We count the frames in the track by stepping through all of its interesting
// times (the places where the track displays a new sample).
//
//////////

long QTUtils_GetFrameCount(Track theTrack)
{
	long myCount = -1;
	short myFlags;
	TimeValue myTime = 0;

	if (theTrack == NULL)
		goto bail;

	// we want to begin with the first frame (sample) in the track
	myFlags = nextTimeMediaSample + nextTimeEdgeOK;

	while (myTime >= 0) {
		myCount++;

		// look for the next frame in the track; when there are no more frames,
		// myTime is set to -1, so we'll exit the while loop
		GetTrackNextInterestingTime(
			theTrack, myFlags, myTime, fixed1, &myTime, NULL);

		// after the first interesting time, don't include the time we're
		// currently at
		myFlags = nextTimeStep;
	}

bail:
	return (myCount);
}

//////////
//
// QTUtils_GetMaxWindowDepth
// Get the deepest pixel type and size of the screen area intersected by a
// specified window.
//
//////////

#if TARGET_OS_MAC
void QTUtils_GetMaxWindowDepth(
	CWindowPtr theWindow, short* thePixelType, short* thePixelSize)
{
	Rect myRect;
	Point myPoint = {0, 0};

	// initialize returned values
	*thePixelType = k1MonochromePixelFormat;
	*thePixelSize = 0;

	if (theWindow == NULL)
		return;

	GetWindowPortBounds(theWindow, &myRect);

	// assume the window is the current port
	LocalToGlobal(&myPoint);

	// offset the rectangle to global coordinates
	MacOffsetRect(&myRect, myPoint.h, myPoint.v);

	// get the max data for the global rectangle
	QTUtils_GetMaxScreenDepth(&myRect, thePixelType, thePixelSize);
}
#endif

//////////
//
// QTUtils_GetMaxScreenDepth
// Get the deepest pixel type and size of the screen area intersected by a
// specified rectangle.
//
//////////

void QTUtils_GetMaxScreenDepth(
	Rect* theGlobalRect, short* thePixelType, short* thePixelSize)
{
	GDHandle myGDevice = NULL;
	PixMapHandle myPixMap = NULL;

	myGDevice = GetMaxDevice(theGlobalRect); // get the max device
	if (myGDevice != NULL) {
		// get the pixmap for the max device
		myPixMap = (**myGDevice).gdPMap;
		if (myPixMap != NULL) {
			// extract the interesting info from the pixmap of the device
			*thePixelType = (**myPixMap).pixelType;
			*thePixelSize = (**myPixMap).pixelSize;
		}
	}
}

//////////
//
// QTUtils_GetUsersConnectionSpeed
// Return the connection speed selected by the user in the QuickTime Settings
// control panel; return kDataRate288ModemRate if the user's QuickTime
// preferences cannot be read of if some other error occurs.
//
// Based on code in Ice Floe Dispatch 17 by Mike Dodd.
//
//////////

long QTUtils_GetUsersConnectionSpeed(void)
{
	QTAtomContainer myPrefsContainer = NULL;
	QTAtom myPrefsAtom = 0;
	ConnectionSpeedPrefsRecord myPrefsRec;
	long myDataSize = 0L;
	long mySpeed = kDataRate288ModemRate;
	Ptr myAtomData = NULL;
	OSErr myErr = noErr;

	// you can retrieve the user's QuickTime preferences by calling
	// GetQuickTimePreference; the first parameter indicates the type of
	// preference you want information about, and the second parameter is an
	// atom container that contains the returned preference data
	myErr = GetQuickTimePreference(ConnectionSpeedPrefsType, &myPrefsContainer);
	if (myErr == noErr) {

		// find the atom of the desired type
		myPrefsAtom = QTFindChildByID(myPrefsContainer, kParentAtomIsContainer,
			ConnectionSpeedPrefsType, 1, NULL);
		if (myPrefsAtom != 0) {
			// we found the desired atom in the returned atom container;
			// read the data contained in that atom and verify that the data is
			// of the size we are expecting
			QTGetAtomDataPtr(
				myPrefsContainer, myPrefsAtom, &myDataSize, &myAtomData);

			if (myDataSize == sizeof(ConnectionSpeedPrefsRecord)) {
				// read the connection speed

				// NOTE: the data in this atom is native-endian, so we do not
				// need to perform any endian-swapping when extracting the speed
				// from the atom. (This is an exception to the rule that data in
				// atom containers is always big-endian.)
				myPrefsRec = *(ConnectionSpeedPrefsRecord*)myAtomData;
				mySpeed = myPrefsRec.connectionSpeed;
			}
		}

		QTDisposeAtomContainer(myPrefsContainer);
	}

	return (mySpeed);
}

//////////
//
// QTUtils_SetUsersConnectionSpeed
// Set the connection speed in the QuickTime Settings control panel to the
// specified value.
//
// NOTE: In general, you should let the user decide the connection speed (using
// the QuickTime Settings control panel). In some cases, however, you might need
// to do this programmatically. Also, you should in general use values for
// theSpeed that are enumerated in the header file MoviesFormat.h.
//
// Based on code in Ice Floe Dispatch 17 by Mike Dodd.
//
//////////

OSErr QTUtils_SetUsersConnectionSpeed(long theSpeed)
{
	QTAtomContainer myPrefsContainer = NULL;
	ConnectionSpeedPrefsRecord myPrefsRec;
	OSErr myErr = noErr;

	myErr = QTNewAtomContainer(&myPrefsContainer);
	if (myErr == noErr) {
		// NOTE: the data in this atom is native-endian, so we do not need to
		// perform any endian-swapping when inserting the speed into the atom.
		// (This is an exception to the rule that data in atom containers is
		// always big-endian.)
		myPrefsRec.connectionSpeed = theSpeed;

		myErr = QTInsertChild(myPrefsContainer, kParentAtomIsContainer,
			ConnectionSpeedPrefsType, 1, 0, sizeof(ConnectionSpeedPrefsRecord),
			&myPrefsRec, NULL);
		if (myErr == noErr)
			myErr = SetQuickTimePreference(
				ConnectionSpeedPrefsType, myPrefsContainer);

		QTDisposeAtomContainer(myPrefsContainer);
	}

	return (myErr);
}

#if CONTENT_RATING_AVAIL
//////////
//
// QTUtils_GetUsersContentRating
// Return, through the function's parameters, the content rating and acceptable
// content types selected by the user in the QuickTime Settings control panel;
// return an error if the user's QuickTime preferences cannot be read.
//
// Based on QTUtils_GetUsersConnectionSpeed.
//
//////////

OSErr QTUtils_GetUsersContentRating(UInt32* theType, UInt16* theRating)
{
	QTAtomContainer myPrefsContainer = NULL;
	QTAtom myPrefsAtom = 0;
	ContentRatingPrefsRecord myContentRec;
	long myDataSize = 0L;
	Ptr myAtomData = NULL;
	OSErr myErr = noErr;

	// you can retrieve the user's QuickTime preferences by calling
	// GetQuickTimePreference; the first parameter indicates the type of
	// preference you want information about, and the second parameter is an
	// atom container that contains the returned preference data
	myErr = GetQuickTimePreference(kContentRatingPrefsType, &myPrefsContainer);
	if (myErr == noErr) {

		// find the atom of the desired type
		myPrefsAtom = QTFindChildByID(myPrefsContainer, kParentAtomIsContainer,
			kContentRatingPrefsType, 1, NULL);
		if (myPrefsAtom == 0) {
			// we did not find any such atom in the returned atom container, so
			// we'll return default settings
			*theType = 0L;
			*theRating = kQTContentTVYRating;
		} else {
			// we found the desired atom in the returned atom container;
			// read the data contained in that atom and verify that the data is
			// of the size we are expecting
			myErr = QTGetAtomDataPtr(
				myPrefsContainer, myPrefsAtom, &myDataSize, &myAtomData);

			if (myDataSize != sizeof(ContentRatingPrefsRecord)) {
				// the data in the atom isn't the right size, so it must be
				// corrupt; return default settings
				*theType = 0L;
				*theRating = kQTContentTVYRating;
			} else {
				// everything is fine: read the content information

				// NOTE: the data in this atom is native-endian, so we do not
				// need to perform any endian-swapping when extracting the data
				// from the atom. (This is an exception to the rule that data in
				// atom containers is always big-endian.)

				// WARNING: the format of the data in a content rating atom is,
				// to my knowledge, currently undocumented; the following method
				// of extracting that info is based on empirical investigation.
				myContentRec = *(ContentRatingPrefsRecord*)myAtomData;
				*theType = (UInt32)(~(myContentRec.fContentTypes) & 0x00ff);
				*theRating = myContentRec.fContentRating;
			}
		}

		QTDisposeAtomContainer(myPrefsContainer);
	}

	return (myErr);
}
#endif // #if CONTENT_RATING_AVAIL

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Controller utilities.
//
// Use these functions to manipulate movie controllers.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////
//
// QTUtils_GetControllerType
// Get the controller type of the specified movie.
//
//////////

OSType QTUtils_GetControllerType(Movie theMovie)
{
	UserData myUserData = NULL;
	OSType myType = kUnknownType;
	OSErr myErr = noErr;

	// make sure we've got a movie
	if (theMovie == NULL)
		return (myType);

	myUserData = GetMovieUserData(theMovie);
	if (myUserData != NULL) {
		myErr = GetUserDataItem(myUserData, &myType, sizeof(myType),
			kUserDataMovieControllerType, 0);
		if (myErr == noErr)
			myType = EndianU32_BtoN(myType);
	}

	return (myType);
}

//////////
//
// QTUtils_SetControllerType
// Set the controller type of the specified movie.
//
// This function adds an item to the movie's user data;
// the updated user data is written to the movie file when the movie is next
// updated (by calling AddMovieResource or UpdateMovieResource).
//
// NOTE: This function is intended to set the controller type of a movie you're
// building; to change the controller of an open movie, use
// QTUtils_ChangeControllerType.
//
//////////

OSErr QTUtils_SetControllerType(Movie theMovie, OSType theType)
{
	UserData myUserData;
	OSErr myErr = noErr;

	// make sure we've got a movie
	if (theMovie == NULL)
		return (paramErr);

	// get the movie's user data list
	myUserData = GetMovieUserData(theMovie);
	if (myUserData == NULL)
		return (paramErr);

	theType = EndianU32_NtoB(theType);
	myErr = SetUserDataItem(
		myUserData, &theType, sizeof(theType), kUserDataMovieControllerType, 0);

	return (myErr);
}

//////////
//
// QTUtils_ChangeControllerType
// Change the controller type of the movie that uses the specified controller,
// "on the fly", and return the new movie controller to the caller; if for some
// reason we cannot create a new movie controller, return NULL.
//
//////////

MovieController QTUtils_ChangeControllerType(
	MovieController theMC, OSType theType, long theFlags)
{
	MovieController myMC = NULL;
	Movie myMovie = NULL;
	Rect myRect;
	OSErr myErr = noErr;

	// make sure we've got a movie controller
	if (theMC == NULL)
		return (NULL);

	// get the movie associated with that controller
	myMovie = MCGetMovie(theMC);
	if (myMovie == NULL)
		return (NULL);

	GetMovieBox(myMovie, &myRect);

	// set the new controller type in the movie's user data list
	myErr = QTUtils_SetControllerType(myMovie, theType);
	if (myErr != noErr)
		return (NULL);

	// dispose of the existing controller
	DisposeMovieController(theMC);

	// create a new controller of the specified type
	myMC = NewMovieController(myMovie, &myRect, theFlags);

	return (myMC);
}

#endif // ifndef __QTUtilities__