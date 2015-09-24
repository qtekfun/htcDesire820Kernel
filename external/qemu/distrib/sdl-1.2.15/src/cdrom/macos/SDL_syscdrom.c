/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#ifdef SDL_CDROM_MACOS


#include <Devices.h>
#include <Files.h>
#include <LowMem.h> 

#include "SDL_cdrom.h"
#include "../SDL_syscdrom.h"
#include "SDL_syscdrom_c.h"

#if !defined(LMGetUnitTableEntryCount)
  #define LMGetUnitTableEntryCount()   *(short *)0x01D2
#endif

#define MAX_DRIVES	26	

static long SDL_cdversion = 0;
static struct {
	short		dRefNum;
	short		driveNum;
	long		frames;
	char		name[256];
	Boolean		hasAudio;
	} SDL_cdlist[MAX_DRIVES];
static StringPtr gDriverName = "\p.AppleCD";

static const char *SDL_SYS_CDName(int drive);
static int SDL_SYS_CDOpen(int drive);
static int SDL_SYS_CDGetTOC(SDL_CD *cdrom);
static CDstatus SDL_SYS_CDStatus(SDL_CD *cdrom, int *position);
static int SDL_SYS_CDPlay(SDL_CD *cdrom, int start, int length);
static int SDL_SYS_CDPause(SDL_CD *cdrom);
static int SDL_SYS_CDResume(SDL_CD *cdrom);
static int SDL_SYS_CDStop(SDL_CD *cdrom);
static int SDL_SYS_CDEject(SDL_CD *cdrom);
static void SDL_SYS_CDClose(SDL_CD *cdrom);

static short SDL_SYS_ShortToBCD(short value)
{
	return((value % 10) + (value / 10) * 0x10); 
}

static short SDL_SYS_BCDToShort(short value)
{
	return((value % 0x10) + (value / 0x10) * 10); 
}

int  SDL_SYS_CDInit(void)
{
	SInt16			dRefNum = 0;
	SInt16			first, last;

	SDL_numcds = 0;

	
	if (Gestalt(kGestaltAudioCDSelector, &SDL_cdversion) || 
			!SDL_cdversion) return(0);

	
	SDL_CDcaps.Name = SDL_SYS_CDName;
	SDL_CDcaps.Open = SDL_SYS_CDOpen;
	SDL_CDcaps.GetTOC = SDL_SYS_CDGetTOC;
	SDL_CDcaps.Status = SDL_SYS_CDStatus;
	SDL_CDcaps.Play = SDL_SYS_CDPlay;
	SDL_CDcaps.Pause = SDL_SYS_CDPause;
	SDL_CDcaps.Resume = SDL_SYS_CDResume;
	SDL_CDcaps.Stop = SDL_SYS_CDStop;
	SDL_CDcaps.Eject = SDL_SYS_CDEject;
	SDL_CDcaps.Close = SDL_SYS_CDClose;

	
	first = -1;
	last = 0 - LMGetUnitTableEntryCount();
	for(dRefNum = first; dRefNum >= last; dRefNum--) {
		Str255		driverName;
		StringPtr	namePtr;
		DCtlHandle	deviceEntry;

		deviceEntry = GetDCtlEntry(dRefNum);
		if (! deviceEntry) continue;
		
		
		namePtr = (*deviceEntry)->dCtlFlags & (1L << dRAMBased) ?
				((StringPtr) ((DCtlPtr) deviceEntry)->dCtlDriver + 18) :
				((StringPtr) (*deviceEntry)->dCtlDriver + 18);
		BlockMoveData(namePtr, driverName, namePtr[0]+1);
		if (driverName[0] > gDriverName[0]) driverName[0] = gDriverName[0];
		if (! EqualString(driverName, gDriverName, false, false)) continue;

		
		SDL_cdlist[SDL_numcds].dRefNum = dRefNum;
		BlockMoveData(namePtr + 1, SDL_cdlist[SDL_numcds].name, namePtr[0]);
		SDL_cdlist[SDL_numcds].name[namePtr[0]] = 0;
		SDL_cdlist[SDL_numcds].hasAudio = false;
		SDL_numcds++;
	}
	return(0);
}

static const char *SDL_SYS_CDName(int drive)
{
	return(SDL_cdlist[drive].name);
}

static int get_drivenum(int drive)
{
	QHdr *driveQ = GetDrvQHdr();
	DrvQEl *driveElem;

	
	SDL_cdlist[drive].driveNum = 0;
	if ( driveQ->qTail ) {
		driveQ->qTail->qLink = 0;
	}
	for ( driveElem=(DrvQEl *)driveQ->qHead; driveElem;
	      driveElem = (DrvQEl *)driveElem->qLink ) {
		if ( driveElem->dQRefNum == SDL_cdlist[drive].dRefNum ) {
			SDL_cdlist[drive].driveNum = driveElem->dQDrive;
			break;
		}
	}
	return(SDL_cdlist[drive].driveNum);
}

static int SDL_SYS_CDOpen(int drive)
{
	return(drive);
}

static int SDL_SYS_CDGetTOC(SDL_CD *cdrom)
{
	CDCntrlParam		cdpb;
	CDTrackData			tracks[SDL_MAX_TRACKS];
	long				i, leadout;

	
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kReadTOC;
	cdpb.csParam.words[0] = kGetTrackRange;
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}

	cdrom->numtracks = 
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[1]) - 
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[0]) + 1;
	if ( cdrom->numtracks > SDL_MAX_TRACKS )
		cdrom->numtracks = SDL_MAX_TRACKS;
	cdrom->status = CD_STOPPED;
	cdrom->cur_track = 0; 
	cdrom->cur_frame = 0; 


	
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kReadTOC;
	cdpb.csParam.words[0] = kGetLeadOutArea;
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}

	leadout = MSF_TO_FRAMES(
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[0]),
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[1]),
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[2]));

	
	SDL_memset(tracks, 0, sizeof(tracks));
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kReadTOC;
	cdpb.csParam.words[0] = kGetTrackEntries;	
	* ((long *) (cdpb.csParam.words+1)) = (long) tracks;				
	cdpb.csParam.words[3] = cdrom->numtracks * sizeof(tracks[0]);		
	* ((char *) (cdpb.csParam.words+4)) = 1;	
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}

	
	SDL_cdlist[cdrom->id].hasAudio = false;
	for ( i=0; i<cdrom->numtracks; ++i ) 
		{
		cdrom->track[i].id = i+1;
		if (tracks[i].entry.control & kDataTrackMask)
			cdrom->track[i].type = SDL_DATA_TRACK;
		else
			{
			cdrom->track[i].type = SDL_AUDIO_TRACK;
			SDL_cdlist[SDL_numcds].hasAudio = true;
			}
		
		cdrom->track[i].offset = MSF_TO_FRAMES(
				SDL_SYS_BCDToShort(tracks[i].entry.min),
				SDL_SYS_BCDToShort(tracks[i].entry.min),
				SDL_SYS_BCDToShort(tracks[i].entry.frame));
		cdrom->track[i].length = MSF_TO_FRAMES(
				SDL_SYS_BCDToShort(tracks[i+1].entry.min),
				SDL_SYS_BCDToShort(tracks[i+1].entry.min),
				SDL_SYS_BCDToShort(tracks[i+1].entry.frame)) -
				cdrom->track[i].offset;
		}
	
	
	cdrom->track[i].offset = leadout;
	cdrom->track[i].length = 0;

	return(0);
}

static CDstatus SDL_SYS_CDStatus(SDL_CD *cdrom, int *position)
{
	CDCntrlParam cdpb;
	CDstatus status = CD_ERROR;
	Boolean spinning = false;

	if (position) *position = 0;

	
	if ( ! get_drivenum(cdrom->id) ) {
		return(CD_TRAYEMPTY);
	}
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kReadTOC;
	cdpb.csParam.words[0] = kGetTrackRange;
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(CD_ERROR);
	}

	cdrom->numtracks = 
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[1]) - 
			SDL_SYS_BCDToShort(cdpb.csParam.bytes[0]) + 1;
	if ( cdrom->numtracks > SDL_MAX_TRACKS )
		cdrom->numtracks = SDL_MAX_TRACKS;
	cdrom->cur_track = 0; 
	cdrom->cur_frame = 0; 


	if (1 || SDL_cdlist[cdrom->id].hasAudio) {
		
		SDL_memset(&cdpb, 0, sizeof(cdpb));
		cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
		cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
		cdpb.csCode = kAudioStatus;
		if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
			SDL_SetError("PBControlSync() failed");
			return(-1);
		}
	
		switch(cdpb.csParam.cd.status) {
			case kStatusPlaying:
				status = CD_PLAYING;
				spinning = true;
				break;
			case kStatusPaused:
				status = CD_PAUSED;
				spinning = true;
				break;
			case kStatusMuted:
				status = CD_PLAYING; 
				spinning = true;
				break;
			case kStatusDone:
				status = CD_STOPPED;
				spinning = true;
				break;
			case kStatusStopped:
				status = CD_STOPPED;
				spinning = false;
				break;
			case kStatusError:
			default:
				status = CD_ERROR;
				spinning = false;
				break;
			}

		if (spinning && position) *position = MSF_TO_FRAMES(
				SDL_SYS_BCDToShort(cdpb.csParam.cd.minute),
				SDL_SYS_BCDToShort(cdpb.csParam.cd.second),
				SDL_SYS_BCDToShort(cdpb.csParam.cd.frame));
		}
	else
		status = CD_ERROR; 

	return(status);
}

static int SDL_SYS_CDPlay(SDL_CD *cdrom, int start, int length)
{
	CDCntrlParam cdpb;

	
	if ( SDL_SYS_CDPause(cdrom) < 0 ) {
		return(-1);
	}

	
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kSetPlayMode;
	cdpb.csParam.bytes[0] = false;			
	cdpb.csParam.bytes[1] = kPlayModeSequential;	
	
	PBControlSync((ParmBlkPtr) &cdpb);

#if 1
	
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioStop;
	cdpb.csParam.words[0] = kBlockPosition;		
	*(long *) (cdpb.csParam.words + 1) = start+length-1; 
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}

	
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioPlay;
	cdpb.csParam.words[0] = kBlockPosition;			
	*(long *) (cdpb.csParam.words + 1) = start+1;	
	cdpb.csParam.words[3] = false;					
	cdpb.csParam.words[4] = kStereoPlayMode;		
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}
#else
	
	FRAMES_TO_MSF(start+length, &m, &s, &f);
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioStop;
	cdpb.csParam.words[0] = kTrackPosition;			
	cdpb.csParam.words[1] = 0;						
	cdpb.csParam.words[2] = 						
			SDL_SYS_ShortToBCD(cdrom->numtracks);	
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}

	
	FRAMES_TO_MSF(start, &m, &s, &f);
	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioPlay;
	cdpb.csParam.words[0] = kTrackPosition;			
	cdpb.csParam.words[1] = 0;						
	cdpb.csParam.words[2] = SDL_SYS_ShortToBCD(1);	
	cdpb.csParam.words[3] = false;					
	cdpb.csParam.words[4] = kStereoPlayMode;		
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}
#endif

	return(0);
}

static int SDL_SYS_CDPause(SDL_CD *cdrom)
{
	CDCntrlParam cdpb;

	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioPause;
	cdpb.csParam.words[0] = 0;	
	cdpb.csParam.words[1] = 1;	
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}
	return(0);
}

static int SDL_SYS_CDResume(SDL_CD *cdrom)
{
	CDCntrlParam cdpb;

	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioPause;
	cdpb.csParam.words[0] = 0;	
	cdpb.csParam.words[1] = 0;	
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}
	return(0);
}

static int SDL_SYS_CDStop(SDL_CD *cdrom)
{
	CDCntrlParam cdpb;

	SDL_memset(&cdpb, 0, sizeof(cdpb));
	cdpb.ioVRefNum = SDL_cdlist[cdrom->id].driveNum;
	cdpb.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
	cdpb.csCode = kAudioStop;
	cdpb.csParam.words[0] = 0;		
	cdpb.csParam.words[1] = 0;		
	cdpb.csParam.words[2] = 0;		
	if ( PBControlSync((ParmBlkPtr)&cdpb) != noErr ) {
		SDL_SetError("PBControlSync() failed");
		return(-1);
	}
	return(0);
}

static int SDL_SYS_CDEject(SDL_CD *cdrom)
{
	Boolean	disk = false;
	QHdr *driveQ = GetDrvQHdr();
	DrvQEl *driveElem;
	HParamBlockRec hpb;
	ParamBlockRec cpb;

	for ( driveElem = (DrvQEl *) driveQ->qHead; driveElem; driveElem = 
			  (driveElem) ? ((DrvQEl *) driveElem->qLink) : 
			  ((DrvQEl *) driveQ->qHead) ) {
		if ( driveQ->qTail ) {
			driveQ->qTail->qLink = 0;
		}
		if ( driveElem->dQRefNum != SDL_cdlist[cdrom->id].dRefNum ) {
			continue;
		}
	
		
		SDL_memset(&hpb, 0, sizeof(hpb));
		hpb.volumeParam.ioVRefNum = driveElem->dQDrive;
		if ( PBHGetVInfoSync(&hpb) != noErr ) {
			continue;
		}
		if ( (UnmountVol(0, driveElem->dQDrive) == noErr) && 
		     (Eject(0, driveElem->dQDrive) == noErr) ) {
			driveElem = 0; 
			disk = true;
		}
	}

	
	if (! disk) {
		SDL_memset(&cpb, 0, sizeof(cpb));
		cpb.cntrlParam.ioVRefNum = 0; 
		cpb.cntrlParam.ioCRefNum = SDL_cdlist[cdrom->id].dRefNum;
		cpb.cntrlParam.csCode = kEjectTheDisc;
		if ( PBControlSync((ParmBlkPtr)&cpb) != noErr ) {
			SDL_SetError("PBControlSync() failed");
			return(-1);
		}
	}
	return(0);
}

static void SDL_SYS_CDClose(SDL_CD *cdrom)
{
	return;
}

void SDL_SYS_CDQuit(void)
{
	while(SDL_numcds--) 
		SDL_memset(SDL_cdlist + SDL_numcds, 0, sizeof(SDL_cdlist[0]));
}

#endif 
