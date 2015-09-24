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

#ifndef _SDL_Atari_eddi_s_h
#define _SDL_Atari_eddi_s_h



#define EDDI_10	(0x0100)
#define EDDI_11 (0x0110)


enum {
	VDI_FORMAT_UNKNOWN=-1,
	VDI_FORMAT_INTER=0,	
	VDI_FORMAT_VDI=1,	
	VDI_FORMAT_PACK=2	
};

enum {
	VDI_CLUT_NONE=0,	
	VDI_CLUT_HARDWARE,	
	VDI_CLUT_SOFTWARE	
};


unsigned long Atari_get_EdDI_version(void *function_pointer);

#endif 
