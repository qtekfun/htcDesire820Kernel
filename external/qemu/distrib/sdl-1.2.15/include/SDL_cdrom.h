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


#ifndef _SDL_cdrom_h
#define _SDL_cdrom_h

#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif


#define SDL_MAX_TRACKS	99

#define SDL_AUDIO_TRACK	0x00
#define SDL_DATA_TRACK	0x04

typedef enum {
	CD_TRAYEMPTY,
	CD_STOPPED,
	CD_PLAYING,
	CD_PAUSED,
	CD_ERROR = -1
} CDstatus;

#define CD_INDRIVE(status)	((int)(status) > 0)

typedef struct SDL_CDtrack {
	Uint8 id;		
	Uint8 type;		
	Uint16 unused;
	Uint32 length;		
	Uint32 offset;		
} SDL_CDtrack;

typedef struct SDL_CD {
	int id;			
	CDstatus status;	

	
        
	int numtracks;		
	int cur_track;		
	int cur_frame;		
	SDL_CDtrack track[SDL_MAX_TRACKS+1];
        
} SDL_CD;

#define CD_FPS	75
#define FRAMES_TO_MSF(f, M,S,F)	{					\
	int value = f;							\
	*(F) = value%CD_FPS;						\
	value /= CD_FPS;						\
	*(S) = value%60;						\
	value /= 60;							\
	*(M) = value;							\
}
#define MSF_TO_FRAMES(M, S, F)	((M)*60*CD_FPS+(S)*CD_FPS+(F))


extern DECLSPEC int SDLCALL SDL_CDNumDrives(void);

extern DECLSPEC const char * SDLCALL SDL_CDName(int drive);

extern DECLSPEC SDL_CD * SDLCALL SDL_CDOpen(int drive);

extern DECLSPEC CDstatus SDLCALL SDL_CDStatus(SDL_CD *cdrom);

extern DECLSPEC int SDLCALL SDL_CDPlayTracks(SDL_CD *cdrom,
		int start_track, int start_frame, int ntracks, int nframes);

extern DECLSPEC int SDLCALL SDL_CDPlay(SDL_CD *cdrom, int start, int length);

extern DECLSPEC int SDLCALL SDL_CDPause(SDL_CD *cdrom);

extern DECLSPEC int SDLCALL SDL_CDResume(SDL_CD *cdrom);

extern DECLSPEC int SDLCALL SDL_CDStop(SDL_CD *cdrom);

extern DECLSPEC int SDLCALL SDL_CDEject(SDL_CD *cdrom);

extern DECLSPEC void SDLCALL SDL_CDClose(SDL_CD *cdrom);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 
