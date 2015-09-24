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


#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audio_c.h"
#include "SDL_dart.h"

#define BUFFER_EMPTY       0
#define BUFFER_USED        1

typedef struct _tMixBufferDesc {
  int              iBufferUsage;      
  SDL_AudioDevice *pSDLAudioDevice;
} tMixBufferDesc, *pMixBufferDesc;


LONG APIENTRY DARTEventFunc(ULONG ulStatus,
			    PMCI_MIX_BUFFER pBuffer,
			    ULONG ulFlags)
{
  if (ulFlags && MIX_WRITE_COMPLETE)
  { 

    
    pMixBufferDesc pBufDesc;

    if (pBuffer)
    {
      pBufDesc = (pMixBufferDesc) (*pBuffer).ulUserParm;

      if (pBufDesc)
      {
        SDL_AudioDevice *pSDLAudioDevice = pBufDesc->pSDLAudioDevice;
        
        pBufDesc->iBufferUsage = BUFFER_EMPTY;
        
        if (pSDLAudioDevice)
        DosPostEventSem(pSDLAudioDevice->hidden->hevAudioBufferPlayed);
      }
    }
  }
  return TRUE;
}


int DART_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
  Uint16 test_format = SDL_FirstAudioFormat(spec->format);
  int valid_datatype = 0;
  MCI_AMP_OPEN_PARMS AmpOpenParms;
  MCI_GENERIC_PARMS GenericParms;
  int iDeviceOrd = 0; 
  int bOpenShared = 1; 
  int iBits = 16; 
  int iFreq = 44100; 
  int iChannels = 2; 
  int iNumBufs = 2;  
  int iBufSize;
  int iOpenMode;
  int iSilence;
  int rc;

  
  SDL_memset(&AmpOpenParms, 0, sizeof(MCI_AMP_OPEN_PARMS));
  
  AmpOpenParms.pszDeviceType = (PSZ) (MCI_DEVTYPE_AUDIO_AMPMIX | (iDeviceOrd << 16));

  iOpenMode = MCI_WAIT | MCI_OPEN_TYPE_ID;
  if (bOpenShared) iOpenMode |= MCI_OPEN_SHAREABLE;

  rc = mciSendCommand( 0, MCI_OPEN,
                       iOpenMode,
		       (PVOID) &AmpOpenParms, 0);
  if (rc!=MCIERR_SUCCESS) 
    return (-1);
  
  
  iDeviceOrd = AmpOpenParms.usDeviceID;

  
  if (spec->channels > 2)
    spec->channels = 2;  

  while ((!valid_datatype) && (test_format)) {
    spec->format = test_format;
    valid_datatype = 1;
    switch (test_format) {
      case AUDIO_U8:
        
        iSilence = 0x80;
        iBits = 8;
        break;

      case AUDIO_S16LSB:
        
        iSilence = 0x00;
        iBits = 16;
        break;

      default:
        valid_datatype = 0;
        test_format = SDL_NextAudioFormat();
        break;
    }
  }

  if (!valid_datatype) { 
    
    mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
    SDL_SetError("Unsupported audio format");
    return (-1);
  }

  iFreq = spec->freq;
  iChannels = spec->channels;
  
  SDL_CalculateAudioSpec(spec);
  iBufSize = spec->size;

  
  SDL_memset(&(_this->hidden->MixSetupParms), 0, sizeof(MCI_MIXSETUP_PARMS));
  _this->hidden->MixSetupParms.ulBitsPerSample = iBits;
  _this->hidden->MixSetupParms.ulFormatTag = MCI_WAVE_FORMAT_PCM;
  _this->hidden->MixSetupParms.ulSamplesPerSec = iFreq;
  _this->hidden->MixSetupParms.ulChannels = iChannels;
  _this->hidden->MixSetupParms.ulFormatMode = MCI_PLAY;
  _this->hidden->MixSetupParms.ulDeviceType = MCI_DEVTYPE_WAVEFORM_AUDIO;
  _this->hidden->MixSetupParms.pmixEvent = DARTEventFunc;
  rc = mciSendCommand (iDeviceOrd, MCI_MIXSETUP,
                       MCI_WAIT | MCI_MIXSETUP_QUERYMODE,
                       &(_this->hidden->MixSetupParms), 0);
  if (rc!=MCIERR_SUCCESS)
  { 
    
    mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
    SDL_SetError("Audio device doesn't support requested audio format");
    return(-1);
  }
  
  rc = mciSendCommand(iDeviceOrd, MCI_MIXSETUP,
                      MCI_WAIT | MCI_MIXSETUP_INIT,
                      &(_this->hidden->MixSetupParms), 0);
  if (rc!=MCIERR_SUCCESS)
  { 
    
    mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
    SDL_SetError("Audio device could not be set up");
    return(-1);
  }
  
  
  
  _this->hidden->pMixBuffers = (MCI_MIX_BUFFER *) SDL_malloc(sizeof(MCI_MIX_BUFFER)*iNumBufs);
  if (!(_this->hidden->pMixBuffers))
  { 
    
    mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
    SDL_SetError("Not enough memory for audio buffer descriptors");
    return(-1);
  }
  
  
  _this->hidden->BufferParms.ulNumBuffers = iNumBufs;               
  _this->hidden->BufferParms.ulBufferSize = iBufSize;               
  _this->hidden->BufferParms.pBufList = _this->hidden->pMixBuffers; 
  
  rc = mciSendCommand(iDeviceOrd, MCI_BUFFER,
                      MCI_WAIT | MCI_ALLOCATE_MEMORY,
                      &(_this->hidden->BufferParms), 0);
  if ((rc!=MCIERR_SUCCESS) || (iNumBufs != _this->hidden->BufferParms.ulNumBuffers) || (_this->hidden->BufferParms.ulBufferSize==0))
  { 
    
    SDL_free(_this->hidden->pMixBuffers); _this->hidden->pMixBuffers = NULL;
    mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
    SDL_SetError("DART could not allocate buffers");
    return(-1);
  }
  
  {
    int i;
    for (i=0; i<iNumBufs; i++)
    {
      pMixBufferDesc pBufferDesc = (pMixBufferDesc) SDL_malloc(sizeof(tMixBufferDesc));;
      
      if ((!(_this->hidden->pMixBuffers[i].pBuffer)) || (!pBufferDesc))
      { 
        
        
        { int j;
          for (j=0; j<i; j++) SDL_free((void *)(_this->hidden->pMixBuffers[j].ulUserParm));
        }
        
        mciSendCommand(iDeviceOrd, MCI_BUFFER, MCI_WAIT | MCI_DEALLOCATE_MEMORY, &(_this->hidden->BufferParms), 0);
        SDL_free(_this->hidden->pMixBuffers); _this->hidden->pMixBuffers = NULL;
        mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
        SDL_SetError("Error at internal buffer check");
        return(-1);
      }
      pBufferDesc->iBufferUsage = BUFFER_EMPTY;
      pBufferDesc->pSDLAudioDevice = _this;

      _this->hidden->pMixBuffers[i].ulBufferLength = _this->hidden->BufferParms.ulBufferSize;
      _this->hidden->pMixBuffers[i].ulUserParm = (ULONG) pBufferDesc; 
      _this->hidden->pMixBuffers[i].ulFlags = 0; 
                                            
                                            
      SDL_memset(_this->hidden->pMixBuffers[i].pBuffer, iSilence, iBufSize);
    }
  }
  _this->hidden->iNextFreeBuffer = 0;
  _this->hidden->iLastPlayedBuf = -1;
  
  if (DosCreateEventSem(NULL, &(_this->hidden->hevAudioBufferPlayed), 0, FALSE)!=NO_ERROR)
  {
    
    {
      int i;
      for (i=0; i<iNumBufs; i++) SDL_free((void *)(_this->hidden->pMixBuffers[i].ulUserParm));
    }
    mciSendCommand(iDeviceOrd, MCI_BUFFER, MCI_WAIT | MCI_DEALLOCATE_MEMORY, &(_this->hidden->BufferParms), 0);
    SDL_free(_this->hidden->pMixBuffers); _this->hidden->pMixBuffers = NULL;
    mciSendCommand(iDeviceOrd, MCI_CLOSE, MCI_WAIT, &GenericParms, 0);
    SDL_SetError("Could not create event semaphore");
    return(-1);
  }

  
  _this->hidden->iCurrDeviceOrd = iDeviceOrd;
  _this->hidden->iCurrFreq = iFreq;
  _this->hidden->iCurrBits = iBits;
  _this->hidden->iCurrChannels = iChannels;
  _this->hidden->iCurrNumBufs = iNumBufs;
  _this->hidden->iCurrBufSize = iBufSize;

  return (0);
}



void DART_ThreadInit(_THIS)
{
  return;
}

void DART_WaitAudio(_THIS)
{
  int i;
  pMixBufferDesc pBufDesc;
  ULONG ulPostCount;

  DosResetEventSem(_this->hidden->hevAudioBufferPlayed, &ulPostCount);
  
  for (i=0; i<_this->hidden->iCurrNumBufs; i++)
  {
    pBufDesc = (pMixBufferDesc) _this->hidden->pMixBuffers[i].ulUserParm;
    if (pBufDesc->iBufferUsage == BUFFER_EMPTY)
      return;
  }
  
  DosWaitEventSem(_this->hidden->hevAudioBufferPlayed, 1000); 
  return;
}

void DART_PlayAudio(_THIS)
{
  int iFreeBuf = _this->hidden->iNextFreeBuffer;
  pMixBufferDesc pBufDesc;

  pBufDesc = (pMixBufferDesc) _this->hidden->pMixBuffers[iFreeBuf].ulUserParm;
  pBufDesc->iBufferUsage = BUFFER_USED;
  
  _this->hidden->MixSetupParms.pmixWrite(_this->hidden->MixSetupParms.ulMixHandle,
                                        &(_this->hidden->pMixBuffers[iFreeBuf]), 1);

  _this->hidden->iLastPlayedBuf = iFreeBuf;
  iFreeBuf = (iFreeBuf+1) % _this->hidden->iCurrNumBufs;
  _this->hidden->iNextFreeBuffer = iFreeBuf;
}

Uint8 *DART_GetAudioBuf(_THIS)
{
  int iFreeBuf;
  Uint8 *pResult;
  pMixBufferDesc pBufDesc;

  if (_this)
  {
    if (_this->hidden)
    {
      iFreeBuf = _this->hidden->iNextFreeBuffer;
      pBufDesc = (pMixBufferDesc) _this->hidden->pMixBuffers[iFreeBuf].ulUserParm;
      
      if (pBufDesc)
      {
        if (pBufDesc->iBufferUsage == BUFFER_EMPTY)
        {
          pResult = _this->hidden->pMixBuffers[iFreeBuf].pBuffer;
          return pResult; 
        }
      } else
        printf("[DART_GetAudioBuf] : ERROR! pBufDesc = %p\n", pBufDesc);
    } else
      printf("[DART_GetAudioBuf] : ERROR! _this->hidden = %p\n", _this->hidden);
  } else
    printf("[DART_GetAudioBuf] : ERROR! _this = %p\n", _this);
  return NULL;
}

void DART_WaitDone(_THIS)
{
  pMixBufferDesc pBufDesc;
  ULONG ulPostCount;
  APIRET rc;

  pBufDesc = (pMixBufferDesc) _this->hidden->pMixBuffers[_this->hidden->iLastPlayedBuf].ulUserParm;
  rc = NO_ERROR;
  while ((pBufDesc->iBufferUsage != BUFFER_EMPTY) && (rc==NO_ERROR))
  {
    DosResetEventSem(_this->hidden->hevAudioBufferPlayed, &ulPostCount);
    rc = DosWaitEventSem(_this->hidden->hevAudioBufferPlayed, 1000); 
  }
}

void DART_CloseAudio(_THIS)
{
  MCI_GENERIC_PARMS GenericParms;
  int rc;

  
  rc = mciSendCommand(_this->hidden->iCurrDeviceOrd, MCI_STOP, MCI_WAIT, &GenericParms, 0);
  if (rc!=MCIERR_SUCCESS)
  {
#ifdef SFX_DEBUG_BUILD
    printf("Could not stop DART playback!\n");
    fflush(stdout);
#endif
  }

  
  DosCloseEventSem(_this->hidden->hevAudioBufferPlayed);

  
  {
    int i;
    for (i=0; i<_this->hidden->iCurrNumBufs; i++) SDL_free((void *)(_this->hidden->pMixBuffers[i].ulUserParm));
  }

  
  rc = mciSendCommand(_this->hidden->iCurrDeviceOrd, MCI_BUFFER, MCI_WAIT | MCI_DEALLOCATE_MEMORY, &(_this->hidden->BufferParms), 0);

  
  SDL_free(_this->hidden->pMixBuffers); _this->hidden->pMixBuffers = NULL;

  
  rc = mciSendCommand(_this->hidden->iCurrDeviceOrd, MCI_CLOSE, MCI_WAIT, &(GenericParms), 0);
}


int Audio_Available(void)
{
  return(1);
}

void Audio_DeleteDevice(SDL_AudioDevice *device)
{
  SDL_free(device->hidden);
  SDL_free(device);
}

SDL_AudioDevice *Audio_CreateDevice(int devindex)
{
  SDL_AudioDevice *this;

  
  this = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
  if ( this )
  {
    SDL_memset(this, 0, (sizeof *this));
    this->hidden = (struct SDL_PrivateAudioData *)
      SDL_malloc((sizeof *this->hidden));
  }
  if ( (this == NULL) || (this->hidden == NULL) )
  {
    SDL_OutOfMemory();
    if ( this )
      SDL_free(this);
    return(0);
  }
  SDL_memset(this->hidden, 0, (sizeof *this->hidden));

  
  this->OpenAudio = DART_OpenAudio;
  this->ThreadInit = DART_ThreadInit;
  this->WaitAudio = DART_WaitAudio;
  this->PlayAudio = DART_PlayAudio;
  this->GetAudioBuf = DART_GetAudioBuf;
  this->WaitDone = DART_WaitDone;
  this->CloseAudio = DART_CloseAudio;

  this->free = Audio_DeleteDevice;

  return this;
}

AudioBootStrap DART_bootstrap = {
	"dart", "OS/2 Direct Audio RouTines (DART)",
	Audio_Available, Audio_CreateDevice
};

