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

#include <nds.h>
#include "SDL.h"
#include "SDL_endian.h"
#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "SDL_ndsaudio.h"
#include "soundcommon.h"


static int NDS_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void NDS_WaitAudio(_THIS);
static void NDS_PlayAudio(_THIS);
static Uint8 *NDS_GetAudioBuf(_THIS);
static void NDS_CloseAudio(_THIS);


u32 framecounter = 0,soundoffset = 0;
static SDL_AudioDevice *sdl_nds_audiodevice; 


void SoundMixCallback(void *stream,u32 len)
{
	SDL_AudioDevice *audio = (SDL_AudioDevice *)sdl_nds_audiodevice;

	
	SDL_memset(stream, audio->spec.silence, len);

	
	if ( ! audio->enabled )
		return;

	if ( ! audio->paused ) {
		if ( audio->convert.needed ) {
			
			SDL_mutexP(audio->mixer_lock);
			(*audio->spec.callback)(audio->spec.userdata,
				(Uint8 *)audio->convert.buf,audio->convert.len);
			SDL_mutexV(audio->mixer_lock);
			SDL_ConvertAudio(&audio->convert);
			SDL_memcpy(stream,audio->convert.buf,audio->convert.len_cvt);
		} else {
			SDL_mutexP(audio->mixer_lock);
			(*audio->spec.callback)(audio->spec.userdata,
						(Uint8 *)stream, len);
			SDL_mutexV(audio->mixer_lock);
		}
	}
	return;
}
void MixSound(void)
{
	int remain;

	if(soundsystem->format == 8)
	{
		if((soundsystem->soundcursor + soundsystem->numsamples) > soundsystem->buffersize)
		{
			SoundMixCallback(&soundsystem->mixbuffer[soundsystem->soundcursor],soundsystem->buffersize - soundsystem->soundcursor);
			remain = soundsystem->numsamples - (soundsystem->buffersize - soundsystem->soundcursor);
			SoundMixCallback(soundsystem->mixbuffer,remain);
		}
		else
		{
			SoundMixCallback(&soundsystem->mixbuffer[soundsystem->soundcursor],soundsystem->numsamples);
		}
	}
	else
	{
		if((soundsystem->soundcursor + soundsystem->numsamples) > (soundsystem->buffersize >> 1))
		{
			SoundMixCallback(&soundsystem->mixbuffer[soundsystem->soundcursor << 1],(soundsystem->buffersize >> 1) - soundsystem->soundcursor);
			remain = soundsystem->numsamples - ((soundsystem->buffersize >> 1) - soundsystem->soundcursor);
			SoundMixCallback(soundsystem->mixbuffer,remain);
		}
		else
		{
			SoundMixCallback(&soundsystem->mixbuffer[soundsystem->soundcursor << 1],soundsystem->numsamples);
		}
	}
}

void InterruptHandler(void)
{
	framecounter++;
}
void FiFoHandler(void)
{
	u32 command;
	while ( !(REG_IPC_FIFO_CR & (IPC_FIFO_RECV_EMPTY)) ) 
	{
		command = REG_IPC_FIFO_RX;

		switch(command)
		{
		case FIFO_NONE:
			break;
		case UPDATEON_ARM9:
			REG_IME = 0;
			MixSound();
			REG_IME = 1;
			SendCommandToArm7(MIXCOMPLETE_ONARM9);
			break;
		}
	}
}





static int Audio_Available(void)
{
	return(1);
}

static void Audio_DeleteDevice(SDL_AudioDevice *device)
{
}

static SDL_AudioDevice *Audio_CreateDevice(int devindex)
{
	
	SDL_AudioDevice *this;

	
	this = (SDL_AudioDevice *)malloc(sizeof(SDL_AudioDevice));
	if ( this ) {
		SDL_memset(this, 0, (sizeof *this));
		this->hidden = (struct SDL_PrivateAudioData *)
				SDL_malloc((sizeof *this->hidden));
	}
	if ( (this == NULL) || (this->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( this ) {
			SDL_free(this);
		}
		return(0);
	}
	SDL_memset(this->hidden, 0, (sizeof *this->hidden));

	
	this->OpenAudio = NDS_OpenAudio;
	this->WaitAudio = NDS_WaitAudio;
	this->PlayAudio = NDS_PlayAudio;
	this->GetAudioBuf = NDS_GetAudioBuf;
	this->CloseAudio = NDS_CloseAudio;

	this->free = Audio_DeleteDevice;
	return this;
}

AudioBootStrap NDSAUD_bootstrap = {
	"nds", "NDS audio",
	Audio_Available, Audio_CreateDevice
};


void static NDS_WaitAudio(_THIS)
{
	
}

static void NDS_PlayAudio(_THIS)
{
	
	if (this->paused)
		return;
	
}

static Uint8 *NDS_GetAudioBuf(_THIS)
{
	return NULL;
}

static void NDS_CloseAudio(_THIS)
{
 
}

static int NDS_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	
	int format = 0;
	
	
	
	
	
	
	
	switch (spec->format&~0x1000) {
		case AUDIO_S8:
			
			format=8;
			break;
		case AUDIO_U8:
			spec->format ^= 0x80;format=8;
			break;
		case AUDIO_U16:
			
			spec->format ^=0x8000;format=16;
		case AUDIO_S16:
			
			format=16;
			break;
	}
	
	SDL_CalculateAudioSpec(spec);

	
	
	
	
	
	
	

	SDL_NDSAudio_mutex = 0; 
	sdl_nds_audiodevice=this;
	
	irqInit();
	irqSet(IRQ_VBLANK,&InterruptHandler);
	irqSet(IRQ_FIFO_NOT_EMPTY,&FiFoHandler);
	irqEnable(IRQ_FIFO_NOT_EMPTY);
	
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR | IPC_FIFO_RECV_IRQ;



	SoundSystemInit(spec->freq,spec->size,0,format);
	SoundStartMixer();

	
	return(1);
}
