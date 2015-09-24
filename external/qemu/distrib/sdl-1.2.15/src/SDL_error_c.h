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


#ifndef _SDL_error_c_h
#define _SDL_error_c_h

#define ERR_MAX_STRLEN	128
#define ERR_MAX_ARGS	5

typedef struct SDL_error {
	
	int error;

	char key[ERR_MAX_STRLEN];

	
	int argc;
	union {
		void *value_ptr;
#if 0	
		unsigned char value_c;
#endif
		int value_i;
		double value_f;
		char buf[ERR_MAX_STRLEN];
	} args[ERR_MAX_ARGS];
} SDL_error;

#endif 
