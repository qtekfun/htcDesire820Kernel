/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is SDL_free software; you can redistribute it and/or
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


extern struct CDcaps {
	
	const char *(*Name)(int drive);

	
	int (*Open)(int drive);

	int (*GetTOC)(SDL_CD *cdrom);

	CDstatus (*Status)(SDL_CD *cdrom, int *position);

	
	int (*Play)(SDL_CD *cdrom, int start, int len); 

	
	int (*Pause)(SDL_CD *cdrom);

	
	int (*Resume)(SDL_CD *cdrom);

	
	int (*Stop)(SDL_CD *cdrom);

	
	int (*Eject)(SDL_CD *cdrom);

	
	void (*Close)(SDL_CD *cdrom);
} SDL_CDcaps;

extern int SDL_numcds;

extern int  SDL_SYS_CDInit(void);

extern void SDL_SYS_CDQuit(void);

