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

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sched.h>
#include <sys/select.h>
#include <sys/neutrino.h>
#include <sys/asoundlib.h>

#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "SDL_nto_audio.h"

#define DRIVER_NAME "qsa-nto"

#define DEFAULT_CPARAMS_RATE 22050
#define DEFAULT_CPARAMS_VOICES 1
#define DEFAULT_CPARAMS_FRAG_SIZE 4096
#define DEFAULT_CPARAMS_FRAGS_MIN 1
#define DEFAULT_CPARAMS_FRAGS_MAX 1

#define OPEN_FLAGS SND_PCM_OPEN_PLAYBACK

#define QSA_NO_WORKAROUNDS  0x00000000
#define QSA_MMAP_WORKAROUND 0x00000001

struct BuggyCards
{
   char* cardname;
   unsigned long bugtype;
};

#define QSA_WA_CARDS 3

struct BuggyCards buggycards[QSA_WA_CARDS]=
{
   {"Sound Blaster Live!", QSA_MMAP_WORKAROUND},
   {"Vortex 8820",         QSA_MMAP_WORKAROUND},
   {"Vortex 8830",         QSA_MMAP_WORKAROUND},
};

static void NTO_ThreadInit(_THIS);
static int NTO_OpenAudio(_THIS, SDL_AudioSpec* spec);
static void NTO_WaitAudio(_THIS);
static void NTO_PlayAudio(_THIS);
static Uint8* NTO_GetAudioBuf(_THIS);
static void NTO_CloseAudio(_THIS);

static int NTO_CheckBuggyCards(_THIS, unsigned long checkfor)
{
    char scardname[33];
    int it;
    
    if (snd_card_get_name(cardno, scardname, 32)<0)
    {
        return 0;
    }

    for (it=0; it<QSA_WA_CARDS; it++)
    {
       if (SDL_strcmp(buggycards[it].cardname, scardname)==0)
       {
          if (buggycards[it].bugtype==checkfor)
          {
              return 1;
          }
       }
    }

    return 0;
}

static void NTO_ThreadInit(_THIS)
{
   int status;
   struct sched_param param;

   
   status=SchedGet(0, 0, &param);
   param.sched_priority=param.sched_curpriority+15;
   status=SchedSet(0, 0, SCHED_NOCHANGE, &param);
}

static void NTO_InitAudioParams(snd_pcm_channel_params_t* cpars)
{
    SDL_memset(cpars, 0, sizeof(snd_pcm_channel_params_t));

    cpars->channel = SND_PCM_CHANNEL_PLAYBACK;
    cpars->mode = SND_PCM_MODE_BLOCK;
    cpars->start_mode = SND_PCM_START_DATA;
    cpars->stop_mode  = SND_PCM_STOP_STOP;
    cpars->format.format = SND_PCM_SFMT_S16_LE;
    cpars->format.interleave = 1;
    cpars->format.rate = DEFAULT_CPARAMS_RATE;
    cpars->format.voices = DEFAULT_CPARAMS_VOICES;
    cpars->buf.block.frag_size = DEFAULT_CPARAMS_FRAG_SIZE;
    cpars->buf.block.frags_min = DEFAULT_CPARAMS_FRAGS_MIN;
    cpars->buf.block.frags_max = DEFAULT_CPARAMS_FRAGS_MAX;
}

static int NTO_AudioAvailable(void)
{

    int available;
    int rval;
    snd_pcm_t* handle;

    available = 0;
    handle = NULL;

    rval = snd_pcm_open_preferred(&handle, NULL, NULL, OPEN_FLAGS);

    if (rval >= 0)
    {
        available = 1;

        if ((rval = snd_pcm_close(handle)) < 0)
        {
            SDL_SetError("NTO_AudioAvailable(): snd_pcm_close failed: %s\n", snd_strerror(rval));
            available = 0;
        }
    }
    else
    {
        SDL_SetError("NTO_AudioAvailable(): there are no available audio devices.\n");
    }

    return (available);
}

static void NTO_DeleteAudioDevice(SDL_AudioDevice *device)
{
    if ((device)&&(device->hidden))
    {
        SDL_free(device->hidden);
    }
    if (device)
    {
        SDL_free(device);
    }
}

static SDL_AudioDevice* NTO_CreateAudioDevice(int devindex)
{
    SDL_AudioDevice *this;

    
    this = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
    if (this)
    {
        SDL_memset(this, 0, sizeof(SDL_AudioDevice));
        this->hidden = (struct SDL_PrivateAudioData *)SDL_malloc(sizeof(struct SDL_PrivateAudioData));
    }
    if ((this == NULL) || (this->hidden == NULL))
    {
        SDL_OutOfMemory();
        if (this)
        {
            SDL_free(this);
	}
        return (0);
    }
    SDL_memset(this->hidden, 0, sizeof(struct SDL_PrivateAudioData));
    audio_handle = NULL;

    
    this->ThreadInit = NTO_ThreadInit;
    this->OpenAudio = NTO_OpenAudio;
    this->WaitAudio = NTO_WaitAudio;
    this->PlayAudio = NTO_PlayAudio;
    this->GetAudioBuf = NTO_GetAudioBuf;
    this->CloseAudio = NTO_CloseAudio;

    this->free = NTO_DeleteAudioDevice;

    return this;
}

AudioBootStrap QNXNTOAUDIO_bootstrap =
{
    DRIVER_NAME, "QNX6 QSA-NTO Audio",
    NTO_AudioAvailable,
    NTO_CreateAudioDevice
};

static void NTO_WaitAudio(_THIS)
{
    fd_set wfds;
    int selectret;

    FD_ZERO(&wfds);
    FD_SET(audio_fd, &wfds);

    do {
        selectret=select(audio_fd + 1, NULL, &wfds, NULL, NULL);
        switch (selectret)
        {
            case -1:
            case  0: SDL_SetError("NTO_WaitAudio(): select() failed: %s\n", strerror(errno));
                     return;
            default: if (FD_ISSET(audio_fd, &wfds))
                     {
                         return;
                     }
                     break;
        }
    } while(1);
}

static void NTO_PlayAudio(_THIS)
{
    int written, rval;
    int towrite;
    void* pcmbuffer;

    if (!this->enabled)
    {
        return;
    }
    
    towrite = this->spec.size;
    pcmbuffer = pcm_buf;

    
    do {
        written = snd_pcm_plugin_write(audio_handle, pcm_buf, towrite);
        if (written != towrite)
        {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                
                SDL_Delay(1);
                
                towrite -= written;
                pcmbuffer += written * this->spec.channels;
                continue;
            }		
            else
            {
                if ((errno == EINVAL) || (errno == EIO))
                {
                    SDL_memset(&cstatus, 0, sizeof(cstatus));
                    cstatus.channel = SND_PCM_CHANNEL_PLAYBACK;
                    if ((rval = snd_pcm_plugin_status(audio_handle, &cstatus)) < 0)
                    {
                        SDL_SetError("NTO_PlayAudio(): snd_pcm_plugin_status failed: %s\n", snd_strerror(rval));
                        return;
                    }	
                    if ((cstatus.status == SND_PCM_STATUS_UNDERRUN) || (cstatus.status == SND_PCM_STATUS_READY))
                    {
                        if ((rval = snd_pcm_plugin_prepare(audio_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
                        {
                            SDL_SetError("NTO_PlayAudio(): snd_pcm_plugin_prepare failed: %s\n", snd_strerror(rval));
                            return;
                        }
                    }		        		
                    continue;
                }
                else
                {
                    return;
                }
            }
        }
        else
        {
            
            towrite -= written;
            pcmbuffer += written * this->spec.channels;
        }
    } while ((towrite > 0)  && (this->enabled));

    
    if (towrite != 0)
    {
        this->enabled = 0;
    }

    return;
}

static Uint8* NTO_GetAudioBuf(_THIS)
{
    return pcm_buf;
}

static void NTO_CloseAudio(_THIS)
{
    int rval;

    this->enabled = 0;

    if (audio_handle != NULL)
    {
        if ((rval = snd_pcm_plugin_flush(audio_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
        {
            SDL_SetError("NTO_CloseAudio(): snd_pcm_plugin_flush failed: %s\n", snd_strerror(rval));
            return;
        }
        if ((rval = snd_pcm_close(audio_handle)) < 0)
        {
            SDL_SetError("NTO_CloseAudio(): snd_pcm_close failed: %s\n",snd_strerror(rval));
            return;
        }
        audio_handle = NULL;
    }
}

static int NTO_OpenAudio(_THIS, SDL_AudioSpec* spec)
{
    int rval;
    int format;
    Uint16 test_format;
    int found;

    audio_handle = NULL;
    this->enabled = 0;

    if (pcm_buf != NULL)
    {
        SDL_FreeAudioMem(pcm_buf); 
        pcm_buf = NULL;
    }

    
    NTO_InitAudioParams(&cparams);

    
    rval = snd_pcm_open_preferred(&audio_handle, &cardno, &deviceno, OPEN_FLAGS);
    if (rval < 0)
    {
        SDL_SetError("NTO_OpenAudio(): snd_pcm_open failed: %s\n", snd_strerror(rval));
        return (-1);
    }

    if (!NTO_CheckBuggyCards(this, QSA_MMAP_WORKAROUND))
    {
        
        if ((rval = snd_pcm_plugin_set_disable(audio_handle, PLUGIN_DISABLE_MMAP)) < 0)
        {
            SDL_SetError("snd_pcm_plugin_set_disable failed: %s\n", snd_strerror(rval));
            return (-1);
        }
    }

    
    format = 0;
    
    found = 0;

    for (test_format=SDL_FirstAudioFormat(spec->format); !found ;)
    {
        
        switch (test_format)
        {
            case AUDIO_U8:
                           format = SND_PCM_SFMT_U8;
                           found = 1;
                           break;
            case AUDIO_S8:
                           format = SND_PCM_SFMT_S8;
                           found = 1;
                           break;
            case AUDIO_S16LSB:
                           format = SND_PCM_SFMT_S16_LE;
                           found = 1;
                           break;
            case AUDIO_S16MSB:
                           format = SND_PCM_SFMT_S16_BE;
                           found = 1;
                           break;
            case AUDIO_U16LSB:
                           format = SND_PCM_SFMT_U16_LE;
                           found = 1;
                           break;
            case AUDIO_U16MSB:
                           format = SND_PCM_SFMT_U16_BE;
                           found = 1;
                           break;
            default:
                           break;
        }

        if (!found)
        {
            test_format = SDL_NextAudioFormat();
        }
    }

    
    if (test_format == 0)
    {
        SDL_SetError("NTO_OpenAudio(): Couldn't find any hardware audio formats");
        return (-1);
    }

    spec->format = test_format;

    
    cparams.format.format = format;

    
    cparams.format.voices = spec->channels;
	
    
    cparams.format.rate = spec->freq;

    
    rval = snd_pcm_plugin_params(audio_handle, &cparams);
    if (rval < 0)
    {
        SDL_SetError("NTO_OpenAudio(): snd_pcm_channel_params failed: %s\n", snd_strerror(rval));
        return (-1);
    }

    
    SDL_memset(&csetup, 0x00, sizeof(csetup));
    csetup.channel = SND_PCM_CHANNEL_PLAYBACK;
    if (snd_pcm_plugin_setup(audio_handle, &csetup) < 0)
    {
        SDL_SetError("NTO_OpenAudio(): Unable to setup playback channel\n");
        return -1;
    }


    
    SDL_CalculateAudioSpec(spec);

    pcm_len = spec->size;

    if (pcm_len==0)
    {
        pcm_len = csetup.buf.block.frag_size * spec->channels * (snd_pcm_format_width(format)/8);
    }

    pcm_buf = (Uint8*)SDL_AllocAudioMem(pcm_len);
    if (pcm_buf == NULL)
    {
        SDL_SetError("NTO_OpenAudio(): pcm buffer allocation failed\n");
        return (-1);
    }
    SDL_memset(pcm_buf, spec->silence, pcm_len);

    
    if ((audio_fd = snd_pcm_file_descriptor(audio_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
    {
        SDL_SetError("NTO_OpenAudio(): snd_pcm_file_descriptor failed with error code: %s\n", snd_strerror(rval));
        return (-1);
    }

    
    rval = snd_pcm_plugin_prepare(audio_handle, SND_PCM_CHANNEL_PLAYBACK);
    if (rval < 0)
    {
        SDL_SetError("snd_pcm_plugin_prepare failed: %s\n", snd_strerror(rval));
        return (-1);
    }

    this->enabled = 1;

    
    parent = getpid();

    
    return (0);
}
