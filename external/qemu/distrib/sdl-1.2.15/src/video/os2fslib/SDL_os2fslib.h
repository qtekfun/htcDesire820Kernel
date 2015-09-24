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

#ifndef _SDL_os2fslib_h
#define _SDL_os2fslib_h


#define INCL_TYPES
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_WIN
#define INCL_GPI
#include <os2.h>

#include <FSLib.h>

#define _THIS	SDL_VideoDevice *_this

struct SDL_PrivateVideoData
{
  FSLib_VideoMode_p   pAvailableFSLibVideoModes;
  SDL_Rect          **pListModesResult; 

  FSLib_VideoMode     SrcBufferDesc;    
  char               *pchSrcBuffer;     
  SDL_Surface        *pSDLSurface;      
  HMTX                hmtxUseSrcBuffer; 
  HWND                hwndFrame, hwndClient;  
  int                 iPMThreadStatus;  
                                        
                                        
  int                 tidPMThread;      
  int                 fInFocus;         
  int                 iSkipWMMOUSEMOVE; 
  int                 iMouseVisible;    

  PFNWP               pfnOldFrameProc;  
  int                 bProportionalResize; 
                                           
  ULONG               ulResizingFlag;   
};

extern DECLSPEC void SDLCALL SDL_OS2FSLIB_SetFCFToUse(ULONG ulFCF);

#endif 
