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


#ifndef _SDL_xbios_sb3_h_
#define _SDL_xbios_sb3_h_


#ifndef C_SCPN
#define C_SCPN 0x5343504EL
#endif

#define SCPN_DEV_1BPP	0
#define SCPN_DEV_2BPP	1
#define SCPN_DEV_4BPP	2
#define SCPN_DEV_8BPP	3
#define SCPN_DEV_16BPP	4

extern const int SDL_XBIOS_scpn_planes_device[];


typedef struct {
	unsigned short	virtual_width;	
	unsigned short	virtual_height;	
	unsigned short	visible_width;	
	unsigned short	visible_height;	
	unsigned short	h_pos;	
	unsigned short	v_pos;	
	unsigned short	dummy;
	unsigned long	size;	
	unsigned short	device;	
							
} scpn_screeninfo_t;

typedef struct {
	unsigned long	magic;	
	unsigned short	version;
	void			*dummy1;
	unsigned short	ptsout0_1;
	unsigned short	ptsout0_2;
	unsigned short	dummy3;
	unsigned char	date[8];	
	unsigned char	asm_string[30];	
	unsigned short	dummy4;
	scpn_screeninfo_t	*screen_info;
	unsigned short	dummy6;
} scpn_cookie_t;


int SDL_XBIOS_SB3Usable(scpn_cookie_t *cookie_scpn);

void SDL_XBIOS_ListSB3Modes(_THIS, int actually_add, scpn_cookie_t *cookie_scpn);

#endif 
