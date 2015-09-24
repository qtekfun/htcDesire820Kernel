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


#include <process.h>
#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#include <os2.h>

#include "SDL_thread.h"
#include "../SDL_systhread.h"
#include "../SDL_thread_c.h"

typedef struct ThreadStartParms
{
  void *args;
  pfnSDL_CurrentEndThread pfnCurrentEndThread;
} tThreadStartParms, *pThreadStartParms;

static void threadfunc(void *pparm)
{
  pThreadStartParms pThreadParms = pparm;
  pfnSDL_CurrentEndThread pfnCurrentEndThread = NULL;

  
  SDL_RunThread(pThreadParms->args);

  
  if (pThreadParms)
  {
    pfnCurrentEndThread = pThreadParms->pfnCurrentEndThread;
    SDL_free(pThreadParms);
  }
  
  if (pfnCurrentEndThread)
    (*pfnCurrentEndThread)();
}

int SDL_SYS_CreateThread(SDL_Thread *thread, void *args, pfnSDL_CurrentBeginThread pfnBeginThread, pfnSDL_CurrentEndThread pfnEndThread)
{
  pThreadStartParms pThreadParms = SDL_malloc(sizeof(tThreadStartParms));
  if (!pThreadParms)
  {
    SDL_SetError("Not enough memory to create thread");
    return(-1);
  }

  
  pThreadParms->pfnCurrentEndThread = pfnEndThread;
  
  pThreadParms->args = args;
  
  thread->threadid = thread->handle = (*pfnBeginThread)(threadfunc, NULL, 512*1024, pThreadParms);
  if ((int)thread->threadid <= 0)
  {
    SDL_SetError("Not enough resources to create thread");
    return(-1);
  }
  return(0);
}

void SDL_SYS_SetupThread(void)
{
  return;
}

DECLSPEC Uint32 SDLCALL SDL_ThreadID(void)
{
  PTIB tib;
  DosGetInfoBlocks(&tib, NULL);
  return((Uint32) (tib->tib_ptib2->tib2_ultid));
}

void SDL_SYS_WaitThread(SDL_Thread *thread)
{
  TID tid = thread->handle;
  DosWaitThread(&tid, DCWW_WAIT);
}

void SDL_SYS_KillThread(SDL_Thread *thread)
{
  DosKillThread(thread->handle);
}
