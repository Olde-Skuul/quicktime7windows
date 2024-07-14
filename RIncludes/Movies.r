/*
     File:       Movies.r
 
     Contains:   QuickTime Interfaces.
 
     Version:    QuickTime 7.3
 
     Copyright:  � 2007 � 1990-2007 by Apple Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __MOVIES_R__
#define __MOVIES_R__

#ifndef __CONDITIONALMACROS_R__
#include "ConditionalMacros.r"
#endif


type 'qter' {
 longint = $$Countof(ErrorSpec);
    wide array ErrorSpec {
 longint;                            // error code used to find this error
  longint                             // error type
      kQuickTimeErrorNotice = 1,
     kQuickTimeErrorWarning = 2,
        kQuickTimeErrorError = 3;
  // In the following strings, ^FILENAME, ^APPNAME, ^0, ^1, etc will be replaced as appropriate.
 pstring;                            // main error string
   pstring;                            // explaination error string
   pstring;                            // technical string (not displayed to user except in debug cases)
  align long;
    };
};


#endif /* __MOVIES_R__ */

