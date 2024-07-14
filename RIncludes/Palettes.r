/*
     File:       Palettes.r
 
     Contains:   Palette Manager Interfaces.
 
     Version:    QuickTime 7.3
 
     Copyright:  � 2007 � 1987-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __PALETTES_R__
#define __PALETTES_R__

#ifndef __CONDITIONALMACROS_R__
#include "ConditionalMacros.r"
#endif

/*----------------------------pltt � Color Palette--------------------------------------*/
type 'pltt' {
        integer = $$CountOf(ColorInfo);                         /* Color table count    */
        fill long;                                              /* Reserved             */
        fill word;                                              /* Reserved             */
        fill long;                                              /* Reserved             */
        fill long;                                              /* Reserved             */
        wide array ColorInfo {
            unsigned integer;                                   /* RGB: red             */
            unsigned integer;                                   /*      green           */
            unsigned integer;                                   /*      blue            */
            integer     pmCourteous, pmDithered, pmTolerant,    /* Color usage          */
                        pmAnimated = 4, pmExplicit = 8;
            integer;                                            /* Tolerance value      */
            fill word;                                          /* Private flags        */
            fill long;                                          /* Private              */
        };
};

#endif /* __PALETTES_R__ */

