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

    Carsten Griwodz
    griff@kom.tu-darmstadt.de

    based on linux/SDL_dspaudio.c by Sam Lantinga
*/
#include "SDL_config.h"


#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_paudio.h"

#define DEBUG_AUDIO 1

#include <sys/machine.h>
#undef BIG_ENDIAN
#include <sys/audio.h>

#define Paud_DRIVER_NAME         "paud"

#define OPEN_FLAGS	O_WRONLY

static int Paud_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void Paud_WaitAudio(_THIS);
static void Paud_PlayAudio(_THIS);
static Uint8 *Paud_GetAudioBuf(_THIS);
static void Paud_CloseAudio(_THIS);


static int Audio_Available(void)
{
	int fd;
	int available;

	available = 0;
	fd = SDL_OpenAudioPath(NULL, 0, OPEN_FLAGS, 0);
	if ( fd >= 0 ) {
		available = 1;
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

	
	this->OpenAudio = Paud_OpenAudio;
	this->WaitAudio = Paud_WaitAudio;
	this->PlayAudio = Paud_PlayAudio;
	this->GetAudioBuf = Paud_GetAudioBuf;
	this->CloseAudio = Paud_CloseAudio;

	this->free = Audio_DeleteDevice;

	return this;
}

AudioBootStrap Paud_bootstrap = {
	Paud_DRIVER_NAME, "AIX Paudio",
	Audio_Available, Audio_CreateDevice
};

static void Paud_WaitAudio(_THIS)
{
    fd_set fdset;

    
    if ( frame_ticks ) {
        
        Sint32 ticks;

        ticks = ((Sint32)(next_frame - SDL_GetTicks()))-FUDGE_TICKS;
        if ( ticks > 0 ) {
	    SDL_Delay(ticks);
        }
    } else {
        audio_buffer  paud_bufinfo;

        
        struct timeval timeout;
        FD_ZERO(&fdset);
        FD_SET(audio_fd, &fdset);

        if ( ioctl(audio_fd, AUDIO_BUFFER, &paud_bufinfo) < 0 ) {
#ifdef DEBUG_AUDIO
            fprintf(stderr, "Couldn't get audio buffer information\n");
#endif
            timeout.tv_sec  = 10;
            timeout.tv_usec = 0;
        } else {
	    long ms_in_buf = paud_bufinfo.write_buf_time;
            timeout.tv_sec  = ms_in_buf/1000;
	    ms_in_buf       = ms_in_buf - timeout.tv_sec*1000;
            timeout.tv_usec = ms_in_buf*1000;
#ifdef DEBUG_AUDIO
            fprintf( stderr,
		     "Waiting for write_buf_time=%ld,%ld\n",
		     timeout.tv_sec,
		     timeout.tv_usec );
#endif
	}

#ifdef DEBUG_AUDIO
        fprintf(stderr, "Waiting for audio to get ready\n");
#endif
        if ( select(audio_fd+1, NULL, &fdset, NULL, &timeout) <= 0 ) {
            const char *message = "Audio timeout - buggy audio driver? (disabled)";
            fprintf(stderr, "SDL: %s - %s\n", strerror(errno), message);
            this->enabled = 0;
            
            audio_fd = -1;
#ifdef DEBUG_AUDIO
            fprintf(stderr, "Done disabling audio\n");
#endif
        }
#ifdef DEBUG_AUDIO
        fprintf(stderr, "Ready!\n");
#endif
    }
}

static void Paud_PlayAudio(_THIS)
{
	int written;

	
	do {
		written = write(audio_fd, mixbuf, mixlen);
		if ( (written < 0) && ((errno == 0) || (errno == EAGAIN)) ) {
			SDL_Delay(1);	
		}
	} while ( (written < 0) && 
	          ((errno == 0) || (errno == EAGAIN) || (errno == EINTR)) );

	
	if ( frame_ticks ) {
		next_frame += frame_ticks;
	}

	
	if ( written < 0 ) {
		this->enabled = 0;
	}
#ifdef DEBUG_AUDIO
	fprintf(stderr, "Wrote %d bytes of audio data\n", written);
#endif
}

static Uint8 *Paud_GetAudioBuf(_THIS)
{
	return mixbuf;
}

static void Paud_CloseAudio(_THIS)
{
	if ( mixbuf != NULL ) {
		SDL_FreeAudioMem(mixbuf);
		mixbuf = NULL;
	}
	if ( audio_fd >= 0 ) {
		close(audio_fd);
		audio_fd = -1;
	}
}

static int Paud_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	char          audiodev[1024];
	int           format;
	int           bytes_per_sample;
	Uint16        test_format;
	audio_init    paud_init;
	audio_buffer  paud_bufinfo;
	audio_status  paud_status;
	audio_control paud_control;
	audio_change  paud_change;

	
	frame_ticks = 0.0;

	
	audio_fd = SDL_OpenAudioPath(audiodev, sizeof(audiodev), OPEN_FLAGS, 0);
	if ( audio_fd < 0 ) {
		SDL_SetError("Couldn't open %s: %s", audiodev, strerror(errno));
		return -1;
	}

	if ( ioctl(audio_fd, AUDIO_BUFFER, &paud_bufinfo) < 0 ) {
		SDL_SetError("Couldn't get audio buffer information");
		return -1;
	}

	mixbuf = NULL;

	if ( spec->channels > 1 )
	    spec->channels = 2;
	else
	    spec->channels = 1;


        paud_init.srate = spec->freq;
	paud_init.mode = PCM;
	paud_init.operation = PLAY;
	paud_init.channels = spec->channels;

	
	format = 0;
	for ( test_format = SDL_FirstAudioFormat(spec->format);
						! format && test_format; ) {
#ifdef DEBUG_AUDIO
		fprintf(stderr, "Trying format 0x%4.4x\n", test_format);
#endif
		switch ( test_format ) {
			case AUDIO_U8:
			    bytes_per_sample = 1;
			    paud_init.bits_per_sample = 8;
			    paud_init.flags = TWOS_COMPLEMENT | FIXED;
			    format = 1;
			    break;
			case AUDIO_S8:
			    bytes_per_sample = 1;
			    paud_init.bits_per_sample = 8;
			    paud_init.flags = SIGNED |
					      TWOS_COMPLEMENT | FIXED;
			    format = 1;
			    break;
			case AUDIO_S16LSB:
			    bytes_per_sample = 2;
			    paud_init.bits_per_sample = 16;
			    paud_init.flags = SIGNED |
					      TWOS_COMPLEMENT | FIXED;
			    format = 1;
			    break;
			case AUDIO_S16MSB:
			    bytes_per_sample = 2;
			    paud_init.bits_per_sample = 16;
			    paud_init.flags = BIG_ENDIAN |
					      SIGNED |
					      TWOS_COMPLEMENT | FIXED;
			    format = 1;
			    break;
			case AUDIO_U16LSB:
			    bytes_per_sample = 2;
			    paud_init.bits_per_sample = 16;
			    paud_init.flags = TWOS_COMPLEMENT | FIXED;
			    format = 1;
			    break;
			case AUDIO_U16MSB:
			    bytes_per_sample = 2;
			    paud_init.bits_per_sample = 16;
			    paud_init.flags = BIG_ENDIAN |
					      TWOS_COMPLEMENT | FIXED;
			    format = 1;
			    break;
			default:
				break;
		}
		if ( ! format ) {
			test_format = SDL_NextAudioFormat();
		}
	}
	if ( format == 0 ) {
#ifdef DEBUG_AUDIO
            fprintf(stderr, "Couldn't find any hardware audio formats\n");
#endif
	    SDL_SetError("Couldn't find any hardware audio formats");
	    return -1;
	}
	spec->format = test_format;

	if ( paud_bufinfo.request_buf_cap == 1 )
	{
	    spec->samples = paud_bufinfo.write_buf_cap
			  / bytes_per_sample
			  / spec->channels;
	}
	else
	{
	    spec->samples = paud_bufinfo.write_buf_cap
			  / bytes_per_sample
			  / spec->channels
			  / 2;
	}
        paud_init.bsize = bytes_per_sample * spec->channels;

	SDL_CalculateAudioSpec(spec);

	if ( ioctl(audio_fd, AUDIO_INIT, &paud_init) < 0 ) {
	    switch ( paud_init.rc )
	    {
	    case 1 :
		SDL_SetError("Couldn't set audio format: DSP can't do play requests");
		return -1;
		break;
	    case 2 :
		SDL_SetError("Couldn't set audio format: DSP can't do record requests");
		return -1;
		break;
	    case 4 :
		SDL_SetError("Couldn't set audio format: request was invalid");
		return -1;
		break;
	    case 5 :
		SDL_SetError("Couldn't set audio format: conflict with open's flags");
		return -1;
		break;
	    case 6 :
		SDL_SetError("Couldn't set audio format: out of DSP MIPS or memory");
		return -1;
		break;
	    default :
		SDL_SetError("Couldn't set audio format: not documented in sys/audio.h");
		return -1;
		break;
	    }
	}

	
	mixlen = spec->size;
	mixbuf = (Uint8 *)SDL_AllocAudioMem(mixlen);
	if ( mixbuf == NULL ) {
		return -1;
	}
	SDL_memset(mixbuf, spec->silence, spec->size);

	paud_change.input = AUDIO_IGNORE;         
        paud_change.output = OUTPUT_1;            
        paud_change.monitor = AUDIO_IGNORE;       
        paud_change.volume = 0x7fffffff;          
        paud_change.volume_delay = AUDIO_IGNORE;  
        paud_change.balance = 0x3fffffff;         
        paud_change.balance_delay = AUDIO_IGNORE; 
        paud_change.treble = AUDIO_IGNORE;        
        paud_change.bass = AUDIO_IGNORE;          
        paud_change.pitch = AUDIO_IGNORE;         

	paud_control.ioctl_request = AUDIO_CHANGE;
	paud_control.request_info = (char*)&paud_change;
	if ( ioctl(audio_fd, AUDIO_CONTROL, &paud_control) < 0 ) {
#ifdef DEBUG_AUDIO
            fprintf(stderr, "Can't change audio display settings\n" );
#endif
	}

	paud_control.ioctl_request = AUDIO_START;
	paud_control.position = 0;
	if ( ioctl(audio_fd, AUDIO_CONTROL, &paud_control) < 0 ) {
#ifdef DEBUG_AUDIO
            fprintf(stderr, "Can't start audio play\n" );
#endif
	    SDL_SetError("Can't start audio play");
	    return -1;
	}

        
        { char *workaround;
                workaround = SDL_getenv("SDL_DSP_NOSELECT");
                if ( workaround ) {
                        frame_ticks = (float)(spec->samples*1000)/spec->freq;
                        next_frame = SDL_GetTicks()+frame_ticks;
                }
        }

	
	parent = getpid();

	
	return 0;
}

