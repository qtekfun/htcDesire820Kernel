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


#ifndef _SDL_mintaudio_mcsh_h
#define _SDL_mintaudio_mcsh_h

typedef struct {
	unsigned short version;	
	unsigned short size;	

	unsigned short play;	
	unsigned short record;	
	unsigned short dsp;		
	unsigned short pint;	
	unsigned short rint;	

	unsigned long res1;		
	unsigned long res2;
	unsigned long res3;
	unsigned long res4;
} cookie_mcsn_t;

enum {
	MCSN_ST=0,
	MCSN_TT,
	MCSN_STE=MCSN_TT,
	MCSN_FALCON,
	MCSN_MAC=MCSN_FALCON
};

#define SETSMPFREQ	7	

#endif 
