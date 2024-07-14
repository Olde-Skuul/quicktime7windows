/*
     File:       Folders.r
 
     Contains:   Folder Manager Interfaces.
 
     Version:    QuickTime 7.3
 
     Copyright:  � 2007 � 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __FOLDERS_R__
#define __FOLDERS_R__

#ifndef __CONDITIONALMACROS_R__
#include "ConditionalMacros.r"
#endif

#define kOnSystemDisk 					(-32768)			/*  previously was 0x8000 but that is an unsigned value whereas vRefNum is signed */
#define kOnAppropriateDisk 				(-32767)			/*  Generally, the same as kOnSystemDisk, but it's clearer that this isn't always the 'boot' disk. */
															/*  Folder Domains - Carbon only.  The constants above can continue to be used, but the folder/volume returned will */
															/*  be from one of the domains below. */
#define kSystemDomain 					(-32766)			/*  Read-only system hierarchy. */
#define kLocalDomain 					(-32765)			/*  All users of a single machine have access to these resources. */
#define kNetworkDomain 					(-32764)			/*  All users configured to use a common network server has access to these resources. */
#define kUserDomain 					(-32763)			/*  Read/write. Resources that are private to the user. */
#define kClassicDomain 					(-32762)			/*  Domain referring to the currently configured Classic System Folder */

#define kCreateFolder 					1
#define kDontCreateFolder 				0

#define kSystemFolderType 				'macs'				/*  the system folder  */
#define kDesktopFolderType 				'desk'				/*  the desktop folder; objects in this folder show on the desk top.  */
#define kSystemDesktopFolderType 		'sdsk'				/*  the desktop folder at the root of the hard drive, never the redirected user desktop folder  */
#define kTrashFolderType 				'trsh'				/*  the trash folder; objects in this folder show up in the trash  */
#define kSystemTrashFolderType 			'strs'				/*  the trash folder at the root of the drive, never the redirected user trash folder  */
#define kWhereToEmptyTrashFolderType 	'empt'				/*  the "empty trash" folder; Finder starts empty from here down  */
#define kPrintMonitorDocsFolderType 	'prnt'				/*  Print Monitor documents  */
#define kStartupFolderType 				'strt'				/*  Finder objects (applications, documents, DAs, aliases, to...) to open at startup go here  */
#define kShutdownFolderType 			'shdf'				/*  Finder objects (applications, documents, DAs, aliases, to...) to open at shutdown go here  */
#define kAppleMenuFolderType 			'amnu'				/*  Finder objects to put into the Apple menu go here  */
#define kControlPanelFolderType 		'ctrl'				/*  Control Panels go here (may contain INITs)  */
#define kSystemControlPanelFolderType 	'sctl'				/*  System control panels folder - never the redirected one, always "Control Panels" inside the System Folder  */
#define kExtensionFolderType 			'extn'				/*  System extensions go here  */
#define kFontsFolderType 				'font'				/*  Fonts go here  */
#define kPreferencesFolderType 			'pref'				/*  preferences for applications go here  */
#define kSystemPreferencesFolderType 	'sprf'				/*  System-type Preferences go here - this is always the system's preferences folder, never a logged in user's  */
#define kTemporaryFolderType 			'temp'				/*  temporary files go here (deleted periodically, but don't rely on it.)  */

#define kExtensionDisabledFolderType 	'extD'
#define kControlPanelDisabledFolderType  'ctrD'
#define kSystemExtensionDisabledFolderType  'macD'
#define kStartupItemsDisabledFolderType  'strD'
#define kShutdownItemsDisabledFolderType  'shdD'
#define kApplicationsFolderType 		'apps'
#define kDocumentsFolderType 			'docs'

															/*  new constants  */
#define kVolumeRootFolderType 			'root'				/*  root folder of a volume  */
#define kChewableItemsFolderType 		'flnt'				/*  items deleted at boot  */
#define kApplicationSupportFolderType 	'asup'				/*  third-party items and folders  */
#define kTextEncodingsFolderType 		'�tex'				/*  encoding tables  */
#define kStationeryFolderType 			'odst'				/*  stationery  */
#define kOpenDocFolderType 				'odod'				/*  OpenDoc root  */
#define kOpenDocShellPlugInsFolderType 	'odsp'				/*  OpenDoc Shell Plug-Ins in OpenDoc folder  */
#define kEditorsFolderType 				'oded'				/*  OpenDoc editors in MacOS Folder  */
#define kOpenDocEditorsFolderType 		'�odf'				/*  OpenDoc subfolder of Editors folder  */
#define kOpenDocLibrariesFolderType 	'odlb'				/*  OpenDoc libraries folder  */
#define kGenEditorsFolderType 			'�edi'				/*  CKH general editors folder at root level of Sys folder  */
#define kHelpFolderType 				'�hlp'				/*  CKH help folder currently at root of system folder  */
#define kInternetPlugInFolderType 		'�net'				/*  CKH internet plug ins for browsers and stuff  */
#define kModemScriptsFolderType 		'�mod'				/*  CKH modem scripts, get 'em OUT of the Extensions folder  */
#define kPrinterDescriptionFolderType 	'ppdf'				/*  CKH new folder at root of System folder for printer descs.  */
#define kPrinterDriverFolderType 		'�prd'				/*  CKH new folder at root of System folder for printer drivers  */
#define kScriptingAdditionsFolderType 	'�scr'				/*  CKH at root of system folder  */
#define kSharedLibrariesFolderType 		'�lib'				/*  CKH for general shared libs.  */
#define kVoicesFolderType 				'fvoc'				/*  CKH macintalk can live here  */
#define kControlStripModulesFolderType 	'sdev'				/*  CKH for control strip modules  */
#define kAssistantsFolderType 			'ast�'				/*  SJF for Assistants (MacOS Setup Assistant, etc)  */
#define kUtilitiesFolderType 			'uti�'				/*  SJF for Utilities folder  */
#define kAppleExtrasFolderType 			'aex�'				/*  SJF for Apple Extras folder  */
#define kContextualMenuItemsFolderType 	'cmnu'				/*  SJF for Contextual Menu items  */
#define kMacOSReadMesFolderType 		'mor�'				/*  SJF for MacOS ReadMes folder  */
#define kALMModulesFolderType 			'walk'				/*  EAS for Location Manager Module files except type 'thng' (within kExtensionFolderType)  */
#define kALMPreferencesFolderType 		'trip'				/*  EAS for Location Manager Preferences (within kPreferencesFolderType; contains kALMLocationsFolderType)  */
#define kALMLocationsFolderType 		'fall'				/*  EAS for Location Manager Locations (within kALMPreferencesFolderType)  */
#define kColorSyncProfilesFolderType 	'prof'				/*  for ColorSync� Profiles  */
#define kThemesFolderType 				'thme'				/*  for Theme data files  */
#define kFavoritesFolderType 			'favs'				/*  Favorties folder for Navigation Services  */
#define kInternetFolderType 			'int�'				/*  Internet folder (root level of startup volume)  */
#define kAppearanceFolderType 			'appr'				/*  Appearance folder (root of system folder)  */
#define kSoundSetsFolderType 			'snds'				/*  Sound Sets folder (in Appearance folder)  */
#define kDesktopPicturesFolderType 		'dtp�'				/*  Desktop Pictures folder (in Appearance folder)  */
#define kInternetSearchSitesFolderType 	'issf'				/*  Internet Search Sites folder  */
#define kFindSupportFolderType 			'fnds'				/*  Find support folder  */
#define kFindByContentFolderType 		'fbcf'				/*  Find by content folder  */
#define kInstallerLogsFolderType 		'ilgf'				/*  Installer Logs folder  */
#define kScriptsFolderType 				'scr�'				/*  Scripts folder  */
#define kFolderActionsFolderType 		'fasf'				/*  Folder Actions Scripts folder  */
#define kLauncherItemsFolderType 		'laun'				/*  Launcher Items folder  */
#define kRecentApplicationsFolderType 	'rapp'				/*  Recent Applications folder  */
#define kRecentDocumentsFolderType 		'rdoc'				/*  Recent Documents folder  */
#define kRecentServersFolderType 		'rsvr'				/*  Recent Servers folder  */
#define kSpeakableItemsFolderType 		'spki'				/*  Speakable Items folder  */
#define kKeychainFolderType 			'kchn'				/*  Keychain folder  */
#define kQuickTimeExtensionsFolderType 	'qtex'				/*  QuickTime Extensions Folder (in Extensions folder)  */
#define kDisplayExtensionsFolderType 	'dspl'				/*  Display Extensions Folder (in Extensions folder)  */
#define kMultiprocessingFolderType 		'mpxf'				/*  Multiprocessing Folder (in Extensions folder)  */
#define kPrintingPlugInsFolderType 		'pplg'				/*  Printing Plug-Ins Folder (in Extensions folder)  */

#define kDomainTopLevelFolderType 		'dtop'				/*  The top-level of a Folder domain, e.g. "/System" */
#define kDomainLibraryFolderType 		'dlib'				/*  the Library subfolder of a particular domain */
#define kColorSyncFolderType 			'sync'				/*  Contains ColorSync-related folders */
#define kColorSyncCMMFolderType 		'ccmm'				/*  ColorSync CMMs */
#define kColorSyncScriptingFolderType 	'cscr'				/*  ColorSync Scripting support */
#define kPrintersFolderType 			'impr'				/*  Contains Printing-related folders */
#define kSpeechFolderType 				'spch'				/*  Contains Speech-related folders */
#define kCarbonLibraryFolderType 		'carb'				/*  Contains Carbon-specific file */
#define kDocumentationFolderType 		'info'				/*  Contains Documentation files (not user documents) */
#define kDeveloperDocsFolderType 		'ddoc'				/*  Contains Developer Documentation files and folders */
#define kDeveloperHelpFolderType 		'devh'				/*  Contains Developer Help related files */
#define kISSDownloadsFolderType 		'issd'				/*  Contains Internet Search Sites downloaded from the Internet */
#define kUserSpecificTmpFolderType 		'utmp'				/*  Contains temporary items created on behalf of the current user */
#define kCachedDataFolderType 			'cach'				/*  Contains various cache files for different clients */
#define kFrameworksFolderType 			'fram'				/*  Contains MacOS X Framework folders      */
#define kPrivateFrameworksFolderType 	'pfrm'				/*  Contains MacOS X Private Framework folders      */
#define kClassicDesktopFolderType 		'sdsk'				/*  MacOS 9 compatible desktop folder - same as  */
															/*  kSystemDesktopFolderType but with a more appropriate */
															/*  name for Mac OS X code. */
#define kDeveloperFolderType 			'devf'				/*  Contains MacOS X Developer Resources */
#define kSystemSoundsFolderType 		'ssnd'				/*  Contains Mac OS X System Sound Files */
#define kComponentsFolderType 			'cmpd'				/*  Contains Mac OS X components */
#define kQuickTimeComponentsFolderType 	'wcmp'				/*  Contains QuickTime components for Mac OS X */
#define kCoreServicesFolderType 		'csrv'				/*  Refers to the "CoreServices" folder on Mac OS X */
#define kPictureDocumentsFolderType 	'pdoc'				/*  Refers to the "Pictures" folder in a users home directory */
#define kMovieDocumentsFolderType 		'mdoc'				/*  Refers to the "Movies" folder in a users home directory */
#define kMusicDocumentsFolderType 		'�doc'				/*  Refers to the "Music" folder in a users home directory */
#define kInternetSitesFolderType 		'site'				/*  Refers to the "Sites" folder in a users home directory */
#define kPublicFolderType 				'pubb'				/*  Refers to the "Public" folder in a users home directory */
#define kAudioSupportFolderType 		'adio'				/*  Refers to the Audio support folder for Mac OS X */
#define kAudioSoundsFolderType 			'asnd'				/*  Refers to the Sounds subfolder of Audio Support */
#define kAudioSoundBanksFolderType 		'bank'				/*  Refers to the Banks subfolder of the Sounds Folder */
#define kAudioAlertSoundsFolderType 	'alrt'				/*  Refers to the Alert Sounds subfolder of the Sound Folder */
#define kAudioPlugInsFolderType 		'aplg'				/*  Refers to the Plug-ins subfolder of the Audio Folder    */
#define kAudioComponentsFolderType 		'acmp'				/*  Refers to the Components subfolder of the Audio Plug-ins Folder     */
#define kKernelExtensionsFolderType 	'kext'				/*  Refers to the Kernel Extensions Folder on Mac OS X */
#define kDirectoryServicesFolderType 	'dsrv'				/*  Refers to the Directory Services folder on Mac OS X */
#define kDirectoryServicesPlugInsFolderType  'dplg'			/*  Refers to the Directory Services Plug-Ins folder on Mac OS X  */
#define kInstallerReceiptsFolderType 	'rcpt'				/*  Refers to the "Receipts" folder in Mac OS X */
#define kFileSystemSupportFolderType 	'fsys'				/*  Refers to the [domain]/Library/Filesystems folder in Mac OS X */
#define kAppleShareSupportFolderType 	'shar'				/*  Refers to the [domain]/Library/Filesystems/AppleShare folder in Mac OS X */
#define kAppleShareAuthenticationFolderType  'auth'			/*  Refers to the [domain]/Library/Filesystems/AppleShare/Authentication folder in Mac OS X */
#define kMIDIDriversFolderType 			'midi'				/*  Refers to the MIDI Drivers folder on Mac OS X */
#define kKeyboardLayoutsFolderType 		'klay'				/*  Refers to the [domain]/Library/KeyboardLayouts folder in Mac OS X */
#define kIndexFilesFolderType 			'indx'				/*  Refers to the [domain]/Library/Indexes folder in Mac OS X */
#define kFindByContentIndexesFolderType  'fbcx'				/*  Refers to the [domain]/Library/Indexes/FindByContent folder in Mac OS X */
#define kManagedItemsFolderType 		'mang'				/*  Refers to the Managed Items folder for Mac OS X  */
#define kBootTimeStartupItemsFolderType  'empz'				/*  Refers to the "StartupItems" folder of Mac OS X  */

#define kLocalesFolderType 				'�loc'				/*  PKE for Locales folder  */
#define kFindByContentPluginsFolderType  'fbcp'				/*  Find By Content Plug-ins  */

#define kUsersFolderType 				'usrs'				/*  "Users" folder, contains one folder for each user.  */
#define kCurrentUserFolderType 			'cusr'				/*  The folder for the currently logged on user.  */
#define kCurrentUserRemoteFolderLocation  'rusf'			/*  The remote folder for the currently logged on user  */
#define kCurrentUserRemoteFolderType 	'rusr'				/*  The remote folder location for the currently logged on user  */
#define kSharedUserDataFolderType 		'sdat'				/*  A Shared "Documents" folder, readable & writeable by all users  */
#define kVolumeSettingsFolderType 		'vsfd'				/*  Volume specific user information goes here  */

#define kCreateFolderAtBoot 			0x00000002
#define kCreateFolderAtBootBit 			1
#define kFolderCreatedInvisible 		0x00000004
#define kFolderCreatedInvisibleBit 		2
#define kFolderCreatedNameLocked 		0x00000008
#define kFolderCreatedNameLockedBit 	3
#define kFolderCreatedAdminPrivs 		0x00000010
#define kFolderCreatedAdminPrivsBit 	4

#define kFolderInUserFolder 			0x00000020
#define kFolderInUserFolderBit 			5
#define kFolderTrackedByAlias 			0x00000040
#define kFolderTrackedByAliasBit 		6
#define kFolderInRemoteUserFolderIfAvailable  0x00000080
#define kFolderInRemoteUserFolderIfAvailableBit  7
#define kFolderNeverMatchedInIdentifyFolder  0x00000100
#define kFolderNeverMatchedInIdentifyFolderBit  8
#define kFolderMustStayOnSameVolume 	0x00000200
#define kFolderMustStayOnSameVolumeBit 	9
#define kFolderManagerFolderInMacOS9FolderIfMacOSXIsInstalledMask  0x00000400
#define kFolderManagerFolderInMacOS9FolderIfMacOSXIsInstalledBit  10
#define kFolderInLocalOrRemoteUserFolder  0x000000A0

#define kRelativeFolder 				'relf'
#define kSpecialFolder 					'spcf'

#define kBlessedFolder 					'blsf'
#define kRootFolder 					'rotf'

#define kCurrentUserFolderLocation 		'cusf'				/*     the magic 'Current User' folder location */
															/*     Set this bit to 1 in the .flags field of a FindFolderUserRedirectionGlobals */
															/*     structure if the userName in the struct should be used as the current */
															/*     "User" name */
#define kFindFolderRedirectionFlagUseDistinctUserFoldersBit  0 /*     Set this bit to 1 and the currentUserFolderVRefNum and currentUserFolderDirID */
															/*     fields of the user record will get used instead of finding the user folder */
															/*     with the userName field. */
#define kFindFolderRedirectionFlagUseGivenVRefAndDirIDAsUserFolderBit  1 /*     Set this bit to 1 and the remoteUserFolderVRefNum and remoteUserFolderDirID */
															/*     fields of the user record will get used instead of finding the user folder */
															/*     with the userName field. */
#define kFindFolderRedirectionFlagsUseGivenVRefNumAndDirIDAsRemoteUserFolderBit  2

#define kFolderManagerUserRedirectionGlobalsCurrentVersion  1
#define kFindFolderExtendedFlagsDoNotFollowAliasesBit  0
#define kFindFolderExtendedFlagsDoNotUseUserFolderBit  1
#define kFindFolderExtendedFlagsUseOtherUserRecord  0x01000000

#define kFolderManagerNotificationMessageUserLogIn  'log+'	/*     Sent by system & third party software after a user logs in.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner */
#define kFolderManagerNotificationMessagePreUserLogIn  'logj' /*     Sent by system & third party software before a user logs in.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner */
#define kFolderManagerNotificationMessageUserLogOut  'log-'	/*     Sent by system & third party software before a user logs out.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner */
#define kFolderManagerNotificationMessagePostUserLogOut  'logp' /*     Sent by system & third party software after a user logs out.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner */
#define kFolderManagerNotificationDiscardCachedData  'dche'	/*     Sent by system & third party software when the entire Folder Manager cache should be flushed */
#define kFolderManagerNotificationMessageLoginStartup  'stup' /*     Sent by 'Login' application the first time it starts up after each boot */

#define kDoNotRemoveWhenCurrentApplicationQuitsBit  0
#define kDoNotRemoveWheCurrentApplicationQuitsBit  0		/*     Going away soon, use kDoNotRemoveWheCurrentApplicationQuitsBit */

#define kStopIfAnyNotificationProcReturnsErrorBit  31

/* fld# � list of folder names for Folder Mgr */

    type 'fld#' {
        array {
            literal     longint;                // folder type
            integer     inSystemFolder = 0;     // version
            fill byte;                          // high byte of data length
            pstring;                            // folder name
            align word;
        };
    };

#endif /* __FOLDERS_R__ */

