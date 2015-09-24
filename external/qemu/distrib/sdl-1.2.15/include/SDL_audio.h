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


#ifndef _SDL_audio_h
#define _SDL_audio_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_endian.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"
#include "SDL_rwops.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_AudioSpec {
	int freq;		
	Uint16 format;		
	Uint8  channels;	
	Uint8  silence;		
	Uint16 samples;		
	Uint16 padding;		
	Uint32 size;		
	void (SDLCALL *callback)(void *userdata, Uint8 *stream, int len);
	void  *userdata;
} SDL_AudioSpec;

#define AUDIO_U8	0x0008	
#define AUDIO_S8	0x8008	
#define AUDIO_U16LSB	0x0010	
#define AUDIO_S16LSB	0x8010	
#define AUDIO_U16MSB	0x1010	
#define AUDIO_S16MSB	0x9010	
#define AUDIO_U16	AUDIO_U16LSB
#define AUDIO_S16	AUDIO_S16LSB

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define AUDIO_U16SYS	AUDIO_U16LSB
#define AUDIO_S16SYS	AUDIO_S16LSB
#else
#define AUDIO_U16SYS	AUDIO_U16MSB
#define AUDIO_S16SYS	AUDIO_S16MSB
#endif



typedef struct SDL_AudioCVT {
	int needed;			
	Uint16 src_format;		
	Uint16 dst_format;		
	double rate_incr;		
	Uint8 *buf;			
	int    len;			
	int    len_cvt;			
	int    len_mult;		
	double len_ratio; 	
	void (SDLCALL *filters[10])(struct SDL_AudioCVT *cvt, Uint16 format);
	int filter_index;		
} SDL_AudioCVT;



extern DECLSPEC int SDLCALL SDL_AudioInit(const char *driver_name);
extern DECLSPEC void SDLCALL SDL_AudioQuit(void);

extern DECLSPEC char * SDLCALL SDL_AudioDriverName(char *namebuf, int maxlen);

extern DECLSPEC int SDLCALL SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);

typedef enum {
	SDL_AUDIO_STOPPED = 0,
	SDL_AUDIO_PLAYING,
	SDL_AUDIO_PAUSED
} SDL_audiostatus;

extern DECLSPEC SDL_audiostatus SDLCALL SDL_GetAudioStatus(void);

/**
 * This function pauses and unpauses the audio callback processing.
 * It should be called with a parameter of 0 after opening the audio
 * device to start playing sound.  This is so you can safely initialize
 * data for your callback function after opening the audio device.
 * Silence will be written to the audio device during the pause.
 */
extern DECLSPEC void SDLCALL SDL_PauseAudio(int pause_on);

extern DECLSPEC SDL_AudioSpec * SDLCALL SDL_LoadWAV_RW(SDL_RWops *src, int freesrc, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len);

#define SDL_LoadWAV(file, spec, audio_buf, audio_len) \
	SDL_LoadWAV_RW(SDL_RWFromFile(file, "rb"),1, spec,audio_buf,audio_len)

extern DECLSPEC void SDLCALL SDL_FreeWAV(Uint8 *audio_buf);

extern DECLSPEC int SDLCALL SDL_BuildAudioCVT(SDL_AudioCVT *cvt,
		Uint16 src_format, Uint8 src_channels, int src_rate,
		Uint16 dst_format, Uint8 dst_channels, int dst_rate);

extern DECLSPEC int SDLCALL SDL_ConvertAudio(SDL_AudioCVT *cvt);


#define SDL_MIX_MAXVOLUME 128
extern DECLSPEC void SDLCALL SDL_MixAudio(Uint8 *dst, const Uint8 *src, Uint32 len, int volume);

extern DECLSPEC void SDLCALL SDL_LockAudio(void);
extern DECLSPEC void SDLCALL SDL_UnlockAudio(void);

extern DECLSPEC void SDLCALL SDL_CloseAudio(void);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 
