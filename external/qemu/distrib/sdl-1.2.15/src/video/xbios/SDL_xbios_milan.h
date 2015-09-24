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


#ifndef _SDL_xbios_milan_h
#define _SDL_xbios_milan_h

#include "SDL_xbios.h"


#define MI_MAGIC	0x4D49 

enum {
	CMD_GETMODE=0,
	CMD_SETMODE,
	CMD_GETINFO,
	CMD_ALLOCPAGE,
	CMD_FREEPAGE,
	CMD_FLIPPAGE,
	CMD_ALLOCMEM,
	CMD_FREEMEM,
	CMD_SETADR,
	CMD_ENUMMODES
};

enum {
	ENUMMODE_EXIT=0,
	ENUMMODE_CONT
};

enum {
	BLK_ERR=0,
	BLK_OK,
	BLK_CLEARED
};

#define SCRINFO_OK 1

#define NO_CLUT 0
#define HARD_CLUT 1
#define SOFT_CLUT 2

#define INTERLEAVE_PLANES 0
#define STANDARD_PLANES  1
#define PACKEDPIX_PLANES 2

#define STANDARD_BITS  1
#define FALCON_BITS   2
#define INTEL_BITS   8


typedef struct _scrblk { 
	unsigned long	size;		 
	unsigned long	blk_status;	 
	unsigned long	blk_start;	 
	unsigned long	blk_len;	 
	unsigned long	blk_x;		 
	unsigned long	blk_y;		 
	unsigned long	blk_w;		 
	unsigned long	blk_h;		 
	unsigned long	blk_wrap;	 
} SCRMEMBLK;

typedef struct screeninfo { 
	unsigned long	size;		 
	unsigned long	devID;		 
	unsigned char	name[64];	 
	unsigned long	scrFlags;	 
	unsigned long	frameadr;	 
	unsigned long	scrHeight;	 
	unsigned long	scrWidth;	 
	unsigned long	virtHeight;	 
	unsigned long	virtWidth;	 
	unsigned long	scrPlanes;	 
	unsigned long	scrColors;	 
	unsigned long	lineWrap;	 
	unsigned long	planeWarp;	 
	unsigned long	scrFormat;	 
	unsigned long	scrClut;	 
	unsigned long	redBits;	 
	unsigned long	greenBits;	 
	unsigned long	blueBits;	 
	unsigned long	alphaBits;	 
	unsigned long	genlockBits; 
	unsigned long	unusedBits;	 
	unsigned long	bitFlags;	 
	unsigned long	maxmem;		 
	unsigned long	pagemem;	 
	unsigned long	max_x;		 
	unsigned long	max_y;		 
} SCREENINFO; 


void SDL_XBIOS_ListMilanModes(_THIS, int actually_add);

#endif 
