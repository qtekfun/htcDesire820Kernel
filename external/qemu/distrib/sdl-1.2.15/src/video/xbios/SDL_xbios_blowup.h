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


#ifndef _SDL_xbios_blowup_h
#define _SDL_xbios_blowup_h

#include "SDL_xbios.h"


typedef struct {
	
	unsigned short	enabled;		
	unsigned short	dummy10[6];
	unsigned short	registers_0E;	
	unsigned short	registers_10;	
	unsigned short	dummy11[23];

	
	unsigned short	width;			
	unsigned short	height;			
	unsigned short	dummy20;
	unsigned long	screensize;		
	unsigned short	dummy21[8];
	unsigned short	virtual;		
	unsigned short	virwidth;		
	unsigned short	virheight;		

	unsigned short dummy22;
	unsigned short monitor;			
	unsigned short extension;		
	unsigned short dummy23[13];

	
	unsigned short	dummy30;
	unsigned short	registers_82[6];	
	unsigned short	dummy31[9];

	unsigned short	dummy32;
	unsigned short	registers_A2[6];	
	unsigned short	dummy33[9];

	
	unsigned short	registers_C0;	
	unsigned short	registers_C2;	
	unsigned short	dummy40[30];
} blow_mode_t;

typedef struct {
	blow_mode_t	blowup_modes[10];
	unsigned char	num_mode[6];
	unsigned long	dummy;
	unsigned short	montype;
} blow_cookie_t;


void SDL_XBIOS_ListBlowupModes(_THIS, int actually_add, blow_cookie_t *cookie_blow);

#endif 
