/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"


#define kVerifyTheDisc   	  5		
#define kEjectTheDisc   	  7		
#define kUserEject    		 80		
#define kReadTOC    		100		
#define kReadQ   			101		
#define kAudioTrackSearch   103		
#define kAudioPlay    		104		
#define kAudioPause    		105		
#define kAudioStop    		106		
#define kAudioStatus    	107		
#define kAudioControl    	109		
#define kReadAudioVolume   	112		
#define kSetTrackList   	122		
#define kGetTrackList   	123		
#define kGetTrackIndex   	124		
#define kSetPlayMode   		125		
#define kGetPlayMode   		126		

#define kGetDriveType   	 96		
#define kWhoIsThere    		 97		
#define kGetBlockSize    	 98		
	
#define kBlockPosition    	  0		
#define kAbsMSFPosition    	  1		
#define kTrackPosition    	  2		
#define kIndexPosition    	  3		

#define kMutedPlayMode   	  0		
#define kStereoPlayMode   	  9		

#define kControlFieldMask  	0x0D	
#define kDataTrackMask   	0x04	

#define kGetTrackRange    	  1		
#define kGetLeadOutArea    	  2		
#define kGetTrackEntries   	  3		

#define kStatusPlaying		  0		
#define kStatusPaused		  1		
#define kStatusMuted		  2		
#define kStatusDone			  3		
#define kStatusError		  4		
#define kStatusStopped		  5		

#define kPlayModeSequential	  0		
#define kPlayModeShuffled	  1		
#define kPlayModeProgrammed   2		

#define kGestaltAudioCDSelector    'aucd'
#define kDriverVersion52   		0x00000520
#define kDriverVersion51   		0x00000510
#define kDriverVersion50   		0x00000500

#define kDriveAppleCD_SC   				  1
#define kDriveAppleCD_SCPlus_or_150   	  2
#define kDriveAppleCD_300_or_300Plus   	  3

#define kFirstSCSIDevice   	 -33
#define kLastSCSIDevice    	 -40

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#endif

typedef struct CDCntrlParam {
	QElemPtr				qLink;
	short					qType;
	short					ioTrap;
	Ptr						ioCmdAddr;
	IOCompletionUPP			ioCompletion;
	OSErr					ioResult;
	StringPtr				ioNamePtr;
	short					ioVRefNum;
	short					ioCRefNum;
	short					csCode;
	
	union {
		long				longs[6];
		short				words[11];
		unsigned char		bytes[22];
		struct {
			unsigned char	status;
			unsigned char	play;
			unsigned char	control;
			unsigned char	minute;
			unsigned char	second;
			unsigned char	frame;
			} cd;
		} csParam;

	} CDCntrlParam, *CDCntrlParamPtr;

typedef union CDTrackData {
	long				value;			
	struct {
		unsigned char	reserved : 4;	
		unsigned char	control : 4;	
		unsigned char	min;			
		unsigned char	sec;			
		unsigned char	frame;			
		} entry;						
	} CDTrackData, *CDTrackPtr;
	
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif
