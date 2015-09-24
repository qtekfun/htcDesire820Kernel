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


#define RIFF		0x46464952		
#define WAVE		0x45564157		
#define FACT		0x74636166		
#define LIST		0x5453494c		
#define FMT		0x20746D66		
#define DATA		0x61746164		
#define PCM_CODE	0x0001
#define MS_ADPCM_CODE	0x0002
#define IMA_ADPCM_CODE	0x0011
#define MP3_CODE	0x0055
#define WAVE_MONO	1
#define WAVE_STEREO	2

typedef struct WaveFMT {
	Uint16	encoding;	
	Uint16	channels;		
	Uint32	frequency;		
	Uint32	byterate;		
	Uint16	blockalign;		
	Uint16	bitspersample;		
} WaveFMT;

typedef struct Chunk {
	Uint32 magic;
	Uint32 length;
	Uint8 *data;
} Chunk;

