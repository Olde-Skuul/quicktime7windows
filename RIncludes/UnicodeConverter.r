/*
     File:       UnicodeConverter.r
 
     Contains:   Types, constants, and prototypes for Unicode Converter
 
     Version:    QuickTime 7.3
 
     Copyright:  � 2007 � 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __UNICODECONVERTER_R__
#define __UNICODECONVERTER_R__

#ifndef __CONDITIONALMACROS_R__
#include "ConditionalMacros.r"
#endif

#define kUnicodeUseFallbacksBit 		0
#define kUnicodeKeepInfoBit 			1
#define kUnicodeDirectionalityBits 		2
#define kUnicodeVerticalFormBit 		4
#define kUnicodeLooseMappingsBit 		5
#define kUnicodeStringUnterminatedBit 	6
#define kUnicodeTextRunBit 				7
#define kUnicodeKeepSameEncodingBit 	8
#define kUnicodeForceASCIIRangeBit 		9
#define kUnicodeNoHalfwidthCharsBit 	10
#define kUnicodeTextRunHeuristicsBit 	11
#define kUnicodeMapLineFeedToReturnBit 	12

#define kUnicodeUseFallbacksMask 		0x00000001
#define kUnicodeKeepInfoMask 			0x00000002
#define kUnicodeDirectionalityMask 		0x0000000C
#define kUnicodeVerticalFormMask 		0x00000010
#define kUnicodeLooseMappingsMask 		0x00000020
#define kUnicodeStringUnterminatedMask 	0x00000040
#define kUnicodeTextRunMask 			0x00000080
#define kUnicodeKeepSameEncodingMask 	0x00000100
#define kUnicodeForceASCIIRangeMask 	0x00000200
#define kUnicodeNoHalfwidthCharsMask 	0x00000400
#define kUnicodeTextRunHeuristicsMask 	0x00000800
#define kUnicodeMapLineFeedToReturnMask  0x00001000

#define kUnicodeDefaultDirection 		0
#define kUnicodeLeftToRight 			1
#define kUnicodeRightToLeft 			2

#define kUnicodeDefaultDirectionMask 	0x00
#define kUnicodeLeftToRightMask 		0x04
#define kUnicodeRightToLeftMask 		0x08


#endif /* __UNICODECONVERTER_R__ */

