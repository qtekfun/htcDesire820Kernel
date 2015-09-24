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


#include <stdio.h>
#include <string.h>	
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#if SDL_AUDIO_DRIVER_OSS_SOUNDCARD_H
#include <soundcard.h>
#else
#include <sys/soundcard.h>
#endif

#ifndef MAP_FAILED
#define MAP_FAILED	((Uint8 *)-1)
#endif

#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_dmaaudio.h"

#define DMA_DRIVER_NAME         "dma"

#define OPEN_FLAGS	(O_RDWR|O_NONBLOCK)

static int DMA_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void DMA_WaitAudio(_THIS);
static void DMA_PlayAudio(_THIS);
static Uint8 *DMA_GetAudioBuf(_THIS);
static void DMA_CloseAudio(_THIS);


static int Audio_Available(void)
{
	int available;
	int fd;

	available = 0;

	fd = SDL_OpenAudioPath(NULL, 0, OPEN_FLAGS, 0);
	if ( fd >= 0 ) {
		int caps;
		struct audio_buf_info info;

		if ( (ioctl(fd, SNDCTL_DSP_GETCAPS, &caps) == 0) &&
	             (caps & DSP_CAP_TRIGGER) && (caps & DSP_CAP_MMAP) &&
		     (ioctl(fd, SNDCTL_DSP_GETOSPACE, &info) == 0) ) {
			available = 1;
		}
		close(fd);
	}
	return(available);
}

static void Audio_DeleteDevice(SDL_AudioDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_AudioDevice *Audio_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;

	
	this = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
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
	audio_fd = -1;

	
	this->OpenAudio = DMA_OpenAudio;
	this->WaitAudio = DMA_WaitAudio;
	this->PlayAudio = DMA_PlayAudio;
	this->GetAudioBuf = DMA_GetAudioBuf;
	this->CloseAudio = DMA_CloseAudio;

	this->free = Audio_DeleteDevice;

	return this;
}

AudioBootStrap DMA_bootstrap = {
	DMA_DRIVER_NAME, "OSS /dev/dsp DMA audio",
	Audio_Available, Audio_CreateDevice
};

static void DMA_WaitAudio(_THIS)
{
	fd_set fdset;

	
	{ static int cnt = 0;
		if (parent && (((++cnt)%10) == 0)) { 
			if ( kill(parent, 0) < 0 ) {
				this->enabled = 0;
			}
		}
	}

	
	if ( frame_ticks ) {
		
		Sint32 ticks;

		ticks = ((Sint32)(next_frame - SDL_GetTicks()))-FUDGE_TICKS;
		if ( ticks > 0 ) {
			SDL_Delay(ticks);
		}
	} else {
		
		struct timeval timeout;
		FD_ZERO(&fdset);
		FD_SET(audio_fd, &fdset);
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
#ifdef DEBUG_AUDIO
		fprintf(stderr, "Waiting for audio to get ready\n");
#endif
		if ( select(audio_fd+1, NULL, &fdset, NULL, &timeout) <= 0 ) {
			const char *message =
#ifdef AUDIO_OSPACE_HACK
			"Audio timeout - buggy audio driver? (trying ospace)";
#else
			"Audio timeout - buggy audio driver? (disabled)";
#endif
			fprintf(stderr, "SDL: %s\n", message);
#ifdef AUDIO_OSPACE_HACK
			
			frame_ticks = (float)(this->spec->samples*1000) /
			                      this->spec->freq;
			next_frame = SDL_GetTicks()+frame_ticks;
#else
			this->enabled = 0;
			
			audio_fd = -1;
#ifdef DEBUG_AUDIO
			fprintf(stderr, "Done disabling audio\n");
#endif
#endif 
		}
#ifdef DEBUG_AUDIO
		fprintf(stderr, "Ready!\n");
#endif
	}
}

static void DMA_PlayAudio(_THIS)
{
	
	if ( frame_ticks ) {
		next_frame += frame_ticks;
	}
	return;
}

static Uint8 *DMA_GetAudioBuf(_THIS)
{
	count_info info;
	int playing;
	int filling;

	
	do {
		if ( ioctl(audio_fd, SNDCTL_DSP_GETOPTR, &info) < 0 ) {
			
			this->enabled = 0;
			return(NULL);
		}
	} while ( frame_ticks && (info.blocks < 1) );
#ifdef DEBUG_AUDIO
	if ( info.blocks > 1 ) {
		printf("Warning: audio underflow (%d frags)\n", info.blocks-1);
	}
#endif
	playing = info.ptr / this->spec.size;
	filling = (playing + 1)%num_buffers;
	return (dma_buf + (filling * this->spec.size));
}

static void DMA_CloseAudio(_THIS)
{
	if ( dma_buf != NULL ) {
		munmap(dma_buf, dma_len);
		dma_buf = NULL;
	}
	if ( audio_fd >= 0 ) {
		close(audio_fd);
		audio_fd = -1;
	}
}

static int DMA_ReopenAudio(_THIS, const char *audiodev, int format, int stereo,
							SDL_AudioSpec *spec)
{
	int frag_spec;
	int value;

	
	close(audio_fd);
	audio_fd = open(audiodev, O_RDWR, 0);
	if ( audio_fd < 0 ) {
		SDL_SetError("Couldn't open %s: %s", audiodev, strerror(errno));
		return(-1);
	}

	
	SDL_CalculateAudioSpec(spec);

	
	for ( frag_spec = 0; (0x01<<frag_spec) < spec->size; ++frag_spec );
	if ( (0x01<<frag_spec) != spec->size ) {
		SDL_SetError("Fragment size must be a power of two");
		return(-1);
	}

	
	if ( ioctl(audio_fd, SNDCTL_DSP_SETFRAGMENT, &frag_spec) < 0 ) {
		SDL_SetError("Couldn't set audio fragment spec");
		return(-1);
	}

	
	value = format;
	if ( (ioctl(audio_fd, SNDCTL_DSP_SETFMT, &value) < 0) ||
						(value != format) ) {
		SDL_SetError("Couldn't set audio format");
		return(-1);
	}

	
	value = (spec->channels > 1);
	if ( (ioctl(audio_fd, SNDCTL_DSP_STEREO, &stereo) < 0) ||
						(value != stereo) ) {
		SDL_SetError("Couldn't set audio channels");
		return(-1);
	}

	
	value = spec->freq;
	if ( ioctl(audio_fd, SNDCTL_DSP_SPEED, &value) < 0 ) {
		SDL_SetError("Couldn't set audio frequency");
		return(-1);
	}
	spec->freq = value;

	
	return(0);
}

static int DMA_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	char audiodev[1024];
	int format;
	int stereo;
	int value;
	Uint16 test_format;
	struct audio_buf_info info;

	
	frame_ticks = 0.0;

	
	audio_fd = SDL_OpenAudioPath(audiodev, sizeof(audiodev), OPEN_FLAGS, 0);
	if ( audio_fd < 0 ) {
		SDL_SetError("Couldn't open %s: %s", audiodev, strerror(errno));
		return(-1);
	}
	dma_buf = NULL;
	ioctl(audio_fd, SNDCTL_DSP_RESET, 0);

	
	if ( ioctl(audio_fd, SNDCTL_DSP_GETFMTS, &value) < 0 ) {
		SDL_SetError("Couldn't get audio format list");
		return(-1);
	}

	
	format = 0;
	for ( test_format = SDL_FirstAudioFormat(spec->format);
						! format && test_format; ) {
#ifdef DEBUG_AUDIO
		fprintf(stderr, "Trying format 0x%4.4x\n", test_format);
#endif
		switch ( test_format ) {
			case AUDIO_U8:
				if ( value & AFMT_U8 ) {
					format = AFMT_U8;
				}
				break;
			case AUDIO_S8:
				if ( value & AFMT_S8 ) {
					format = AFMT_S8;
				}
				break;
			case AUDIO_S16LSB:
				if ( value & AFMT_S16_LE ) {
					format = AFMT_S16_LE;
				}
				break;
			case AUDIO_S16MSB:
				if ( value & AFMT_S16_BE ) {
					format = AFMT_S16_BE;
				}
				break;
			case AUDIO_U16LSB:
				if ( value & AFMT_U16_LE ) {
					format = AFMT_U16_LE;
				}
				break;
			case AUDIO_U16MSB:
				if ( value & AFMT_U16_BE ) {
					format = AFMT_U16_BE;
				}
				break;
			default:
				format = 0;
				break;
		}
		if ( ! format ) {
			test_format = SDL_NextAudioFormat();
		}
	}
	if ( format == 0 ) {
		SDL_SetError("Couldn't find any hardware audio formats");
		return(-1);
	}
	spec->format = test_format;

	
	value = format;
	if ( (ioctl(audio_fd, SNDCTL_DSP_SETFMT, &value) < 0) ||
						(value != format) ) {
		SDL_SetError("Couldn't set audio format");
		return(-1);
	}

	
	stereo = (spec->channels > 1);
	ioctl(audio_fd, SNDCTL_DSP_STEREO, &stereo);
	if ( stereo ) {
		spec->channels = 2;
	} else {
		spec->channels = 1;
	}

	if ( DMA_ReopenAudio(this, audiodev, format, stereo, spec) < 0 ) {
		
		return(-1);
	}

	
	if ( ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info) < 0 ) {
		SDL_SetError("Couldn't get OSPACE parameters");
		return(-1);
	}
	spec->size = info.fragsize;
	spec->samples = spec->size / ((spec->format & 0xFF) / 8);
	spec->samples /= spec->channels;
	num_buffers = info.fragstotal;
	dma_len = num_buffers*spec->size;
	dma_buf = (Uint8 *)mmap(NULL, dma_len, PROT_WRITE, MAP_SHARED,
							audio_fd, 0);
	if ( dma_buf == MAP_FAILED ) {
		SDL_SetError("DMA memory map failed");
		dma_buf = NULL;
		return(-1);
	}
	SDL_memset(dma_buf, spec->silence, dma_len);

	
	{ char *workaround;
		workaround = SDL_getenv("SDL_DSP_NOSELECT");
		if ( workaround ) {
			frame_ticks = (float)(spec->samples*1000)/spec->freq;
			next_frame = SDL_GetTicks()+frame_ticks;
		}
	}

	
	value = 0;
	ioctl(audio_fd, SNDCTL_DSP_SETTRIGGER, &value);
	value = PCM_ENABLE_OUTPUT;
	if ( ioctl(audio_fd, SNDCTL_DSP_SETTRIGGER, &value) < 0 ) {
		SDL_SetError("Couldn't trigger audio output");
		return(-1);
	}

	
	parent = getpid();

	
	return(0);
}
