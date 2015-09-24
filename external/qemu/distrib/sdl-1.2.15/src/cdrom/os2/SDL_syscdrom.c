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

#ifdef SDL_CDROM_OS2


#define INCL_MCIOS2
#include <os2.h>
#include <os2me.h>

#include "SDL_cdrom.h"
#include "../SDL_syscdrom.h"

#define MCI_CMDRETBUFSIZE	128

#define MAX_DRIVES	16	

static char *SDL_cdlist[MAX_DRIVES];

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

#define	MCI_MMTIMEPERSECOND		3000
#define	FRAMESFROMMM(mmtime)		(((mmtime)*CD_FPS)/MCI_MMTIMEPERSECOND)


int  SDL_SYS_CDInit(void)
{
int i; 
MCI_SYSINFO_PARMS		msp;	
CHAR 						SysInfoRet[MCI_CMDRETBUFSIZE];	

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

SDL_memset(&msp, 0x00, sizeof(MCI_SYSINFO_PARMS));
msp.usDeviceType = MCI_DEVTYPE_CD_AUDIO;	
msp.pszReturn = (PSZ)&SysInfoRet; 	
msp.ulRetSize = MCI_CMDRETBUFSIZE; 	
if (LOUSHORT(mciSendCommand(0,MCI_SYSINFO, MCI_SYSINFO_QUANTITY | MCI_WAIT, (PVOID)&msp, 0)) != MCIERR_SUCCESS) return(CD_ERROR);
SDL_numcds = atoi(SysInfoRet);
if (SDL_numcds > MAX_DRIVES) SDL_numcds = MAX_DRIVES; 

msp.pszReturn = (PSZ)&SysInfoRet; 				
msp.ulRetSize = MCI_CMDRETBUFSIZE; 			
msp.usDeviceType = MCI_DEVTYPE_CD_AUDIO;		
for (i=0; i<SDL_numcds; i++)
	{
	msp.ulNumber = i+1;
	mciSendCommand(0,MCI_SYSINFO, MCI_SYSINFO_NAME | MCI_WAIT,&msp, 0);
	SDL_cdlist[i] = SDL_strdup(SysInfoRet);
	if ( SDL_cdlist[i] == NULL )
		{
		SDL_OutOfMemory();
		return(-1);
		}
	}
return(0);
}

static const char *SDL_SYS_CDName(int drive)
{
return(SDL_cdlist[drive]);
}

static int SDL_SYS_CDOpen(int drive)
{
MCI_OPEN_PARMS	mop;
MCI_SET_PARMS msp;
MCI_GENERIC_PARMS mgp;

mop.hwndCallback = (HWND)NULL;		
mop.usDeviceID = (USHORT)NULL;		
mop.pszDeviceType = (PSZ)SDL_cdlist[drive];		
if (LOUSHORT(mciSendCommand(0,MCI_OPEN,MCI_WAIT,&mop, 0)) != MCIERR_SUCCESS) return(CD_ERROR);
msp.hwndCallback = (HWND)NULL;		
msp.ulTimeFormat = MCI_FORMAT_MSF;	
msp.ulSpeedFormat = (ULONG)NULL;		
msp.ulAudio = (ULONG)NULL;				
msp.ulLevel = (ULONG)NULL;				
msp.ulOver = (ULONG)NULL;				
msp.ulItem = (ULONG)NULL;				
msp.ulValue = (ULONG)NULL;				
if (LOUSHORT(mciSendCommand(mop.usDeviceID,MCI_SET,MCI_WAIT | MCI_SET_TIME_FORMAT,&msp, 0)) == MCIERR_SUCCESS) return (mop.usDeviceID);
mgp.hwndCallback = (HWND)NULL;		
mciSendCommand(mop.usDeviceID,MCI_CLOSE,MCI_WAIT,&mgp, 0);
return(CD_ERROR);
}

static int SDL_SYS_CDGetTOC(SDL_CD *cdrom)
{
MCI_TOC_PARMS mtp;
MCI_STATUS_PARMS msp;
MCI_TOC_REC * mtr;
INT i;

if (cdrom->status == CD_PLAYING || cdrom->status == CD_PAUSED) return 0;

msp.hwndCallback = (HWND)NULL; 
msp.ulReturn = (ULONG)NULL; 
msp.ulItem = MCI_STATUS_NUMBER_OF_TRACKS;
msp.ulValue = (ULONG)NULL; 
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STATUS,MCI_WAIT | MCI_STATUS_ITEM,&msp, 0)) != MCIERR_SUCCESS) return(CD_ERROR);
cdrom->numtracks = msp.ulReturn;
if ( cdrom->numtracks > SDL_MAX_TRACKS )
	{
	cdrom->numtracks = SDL_MAX_TRACKS;
	}
mtr = (MCI_TOC_REC *)SDL_malloc(cdrom->numtracks*sizeof(MCI_TOC_REC));
if ( mtr == NULL )
	{
	SDL_OutOfMemory();
	return(-1);
	}
mtp.pBuf = mtr;
mtp.ulBufSize = cdrom->numtracks*sizeof(MCI_TOC_REC);
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_GETTOC,MCI_WAIT,&mtp, 0)) != MCIERR_SUCCESS)
	{
	SDL_OutOfMemory();
	SDL_free(mtr);
	return(CD_ERROR);
	}
for (i=0; i<cdrom->numtracks; i++)
	{
	
	cdrom->track[i].id = (mtr+i)->TrackNum;
	
	msp.hwndCallback = (HWND)NULL; 
	msp.ulReturn = (ULONG)NULL; 
	msp.ulItem = MCI_CD_STATUS_TRACK_TYPE;
	msp.ulValue = (ULONG)((mtr+i)->TrackNum); 
	if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STATUS,MCI_WAIT | MCI_TRACK | MCI_STATUS_ITEM,&msp, 0)) != MCIERR_SUCCESS)
		{
		SDL_free(mtr);
		return (CD_ERROR);
		}
	if (msp.ulReturn==MCI_CD_TRACK_AUDIO) cdrom->track[i].type = SDL_AUDIO_TRACK;
	else cdrom->track[i].type = SDL_DATA_TRACK;
	
	cdrom->track[i].length = FRAMESFROMMM((mtr+i)->ulEndAddr - (mtr+i)->ulStartAddr);
	
	cdrom->track[i].offset = FRAMESFROMMM((mtr+i)->ulStartAddr);
	}
SDL_free(mtr);
return(0);
}


static CDstatus SDL_SYS_CDStatus(SDL_CD *cdrom, int *position)
{
CDstatus status;
MCI_STATUS_PARMS msp;

msp.hwndCallback = (HWND)NULL; 
msp.ulReturn = (ULONG)NULL; 
msp.ulItem = MCI_STATUS_MODE;
msp.ulValue = (ULONG)NULL; 
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STATUS,MCI_WAIT | MCI_STATUS_ITEM,&msp, 0)) != MCIERR_SUCCESS) status = CD_ERROR;
else
	{
	switch(msp.ulReturn)
		{
		case	MCI_MODE_NOT_READY:
			status = CD_TRAYEMPTY;
			break;
		case	MCI_MODE_PAUSE:
			status = CD_PAUSED;
			break;
		case	MCI_MODE_PLAY:
			status = CD_PLAYING;
			break;
		case	MCI_MODE_STOP:
			status = CD_STOPPED;
			break;
		
		case	MCI_MODE_RECORD:
		case	MCI_MODE_SEEK:
		default:
			status = CD_ERROR;
			break;
		}
	}

if (position != NULL) 
	{
		if ((status == CD_PLAYING) || (status == CD_PAUSED))
		{
		
		msp.hwndCallback = (HWND)NULL; 
		msp.ulReturn = (ULONG)NULL; 
		msp.ulItem = MCI_STATUS_POSITION;
		msp.ulValue = (ULONG)NULL; 
		if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STATUS,MCI_WAIT | MCI_STATUS_ITEM,&msp, 0)) != MCIERR_SUCCESS) return (CD_ERROR);
		
		*position = MSF_TO_FRAMES(MSF_MINUTE(msp.ulReturn),MSF_SECOND(msp.ulReturn),MSF_FRAME(msp.ulReturn));
		}
	else *position = 0;
	}
return(status);
}

static int SDL_SYS_CDPlay(SDL_CD *cdrom, int start, int length)
{
MCI_GENERIC_PARMS mgp;
MCI_STATUS_PARMS msp;
MCI_PLAY_PARMS	mpp;
ULONG min,sec,frm;

FRAMES_TO_MSF(start, &min, &sec, &frm);
MSF_MINUTE(mpp.ulFrom) = min;
MSF_SECOND(mpp.ulFrom) = sec;
MSF_FRAME(mpp.ulFrom) = frm;
FRAMES_TO_MSF(start+length, &min, &sec, &frm);
MSF_MINUTE(mpp.ulTo) = min;
MSF_SECOND(mpp.ulTo) = sec;
MSF_FRAME(mpp.ulTo) = frm;
#ifdef DEBUG_CDROM
	fprintf(stderr, "Trying to play from %d:%d:%d to %d:%d:%d\n",
	playtime.cdmsf_min0, playtime.cdmsf_sec0, playtime.cdmsf_frame0,
	playtime.cdmsf_min1, playtime.cdmsf_sec1, playtime.cdmsf_frame1);
#endif
msp.hwndCallback = (HWND)NULL; 
msp.ulReturn = (ULONG)NULL; 
msp.ulItem = MCI_STATUS_MODE;
msp.ulValue = (ULONG)NULL; 
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STATUS,MCI_WAIT | MCI_STATUS_ITEM,&msp, 0)) == MCIERR_SUCCESS)
	{
	if (msp.ulReturn == MCI_MODE_PAUSE)
		{
		mgp.hwndCallback = (HWND)NULL;		
		mciSendCommand(cdrom->id,MCI_RESUME,0,&mgp, 0);
		}
	}
mpp.hwndCallback = (HWND)NULL;		
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_PLAY,MCI_FROM | MCI_TO,&mpp, 0)) == MCIERR_SUCCESS) return 0;
return (CD_ERROR);
}

static int SDL_SYS_CDPause(SDL_CD *cdrom)
{
MCI_GENERIC_PARMS mgp;

mgp.hwndCallback = (HWND)NULL;		
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_PAUSE,MCI_WAIT,&mgp, 0)) == MCIERR_SUCCESS) return 0;
return(CD_ERROR);
}

static int SDL_SYS_CDResume(SDL_CD *cdrom)
{
MCI_GENERIC_PARMS mgp;

mgp.hwndCallback = (HWND)NULL;		
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_RESUME,MCI_WAIT,&mgp, 0)) == MCIERR_SUCCESS) return 0;
return(CD_ERROR);
}

static int SDL_SYS_CDStop(SDL_CD *cdrom)
{
MCI_GENERIC_PARMS mgp;
MCI_STATUS_PARMS msp;

msp.hwndCallback = (HWND)NULL; 
msp.ulReturn = (ULONG)NULL; 
msp.ulItem = MCI_STATUS_MODE;
msp.ulValue = (ULONG)NULL; 
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STATUS,MCI_WAIT | MCI_STATUS_ITEM,&msp, 0)) == MCIERR_SUCCESS)
	{
	if (msp.ulReturn == MCI_MODE_PAUSE)
		{
		mgp.hwndCallback = (HWND)NULL;		
		mciSendCommand(cdrom->id,MCI_RESUME,0,&mgp, 0);
		}
	}
mgp.hwndCallback = (HWND)NULL;		
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_STOP,MCI_WAIT,&mgp, 0)) == MCIERR_SUCCESS) return 0;
return(CD_ERROR);
}

static int SDL_SYS_CDEject(SDL_CD *cdrom)
{
MCI_SET_PARMS msp;

msp.hwndCallback = (HWND)NULL;		
msp.ulTimeFormat = (ULONG)NULL;		
msp.ulSpeedFormat = (ULONG)NULL;		
msp.ulAudio = (ULONG)NULL;				
msp.ulLevel = (ULONG)NULL;				
msp.ulOver = (ULONG)NULL;				
msp.ulItem = (ULONG)NULL;					
msp.ulValue = (ULONG)NULL;					
if (LOUSHORT(mciSendCommand(cdrom->id,MCI_SET,MCI_WAIT | MCI_SET_DOOR_OPEN,&msp, 0)) == MCIERR_SUCCESS) return 0;
return(CD_ERROR);
}

static void SDL_SYS_CDClose(SDL_CD *cdrom)
{
MCI_GENERIC_PARMS mgp;

mgp.hwndCallback = (HWND)NULL;		
mciSendCommand(cdrom->id,MCI_CLOSE,MCI_WAIT,&mgp, 0);
}

void SDL_SYS_CDQuit(void)
{
int i;

if ( SDL_numcds > 0 )
	{
	for ( i=0; i<SDL_numcds; ++i )
		{
		SDL_free(SDL_cdlist[i]);
		}
	SDL_numcds = 0;
	}
}

#endif 
