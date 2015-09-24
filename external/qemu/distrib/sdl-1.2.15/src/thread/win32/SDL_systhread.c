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


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "SDL_thread.h"
#include "../SDL_thread_c.h"
#include "../SDL_systhread.h"

#ifndef SDL_PASSED_BEGINTHREAD_ENDTHREAD
#ifndef _WIN32_WCE
#include <process.h>
#endif

#if defined(__WATCOMC__)
#if __WATCOMC__ < 1240
#define __watcall
#endif
typedef unsigned long (__watcall *pfnSDL_CurrentBeginThread) (void *, unsigned,
        unsigned (__stdcall *func)(void *), void *arg, 
        unsigned, unsigned *threadID);
typedef void (__watcall *pfnSDL_CurrentEndThread)(unsigned code);
#elif (defined(__MINGW32__) && (__GNUC__ < 4))
typedef unsigned long (__cdecl *pfnSDL_CurrentBeginThread) (void *, unsigned,
        unsigned (__stdcall *func)(void *), void *arg, 
        unsigned, unsigned *threadID);
typedef void (__cdecl *pfnSDL_CurrentEndThread)(unsigned code);
#else
typedef uintptr_t (__cdecl *pfnSDL_CurrentBeginThread) (void *, unsigned,
        unsigned (__stdcall *func)(void *), void *arg, 
        unsigned, unsigned *threadID);
typedef void (__cdecl *pfnSDL_CurrentEndThread)(unsigned code);
#endif
#endif 


typedef struct ThreadStartParms
{
  void *args;
  pfnSDL_CurrentEndThread pfnCurrentEndThread;
} tThreadStartParms, *pThreadStartParms;

static DWORD RunThread(void *data)
{
  pThreadStartParms pThreadParms = (pThreadStartParms)data;
  pfnSDL_CurrentEndThread pfnCurrentEndThread = NULL;

  
  SDL_RunThread(pThreadParms->args);

  
  if (pThreadParms)
  {
    pfnCurrentEndThread = pThreadParms->pfnCurrentEndThread;
    SDL_free(pThreadParms);
  }
  
  if (pfnCurrentEndThread)
    (*pfnCurrentEndThread)(0);
  return(0);
}

static DWORD WINAPI RunThreadViaCreateThread(LPVOID data)
{
  return RunThread(data);
}

static unsigned __stdcall RunThreadViaBeginThreadEx(void *data)
{
  return (unsigned) RunThread(data);
}

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
int SDL_SYS_CreateThread(SDL_Thread *thread, void *args, pfnSDL_CurrentBeginThread pfnBeginThread, pfnSDL_CurrentEndThread pfnEndThread)
{
#else
int SDL_SYS_CreateThread(SDL_Thread *thread, void *args)
{
#ifdef _WIN32_WCE
	pfnSDL_CurrentBeginThread pfnBeginThread = NULL;
	pfnSDL_CurrentEndThread pfnEndThread = NULL;
#else
	pfnSDL_CurrentBeginThread pfnBeginThread = _beginthreadex;
	pfnSDL_CurrentEndThread pfnEndThread = _endthreadex;
#endif
#endif 
	pThreadStartParms pThreadParms = (pThreadStartParms)SDL_malloc(sizeof(tThreadStartParms));
	if (!pThreadParms) {
		SDL_OutOfMemory();
		return(-1);
	}

	
	pThreadParms->pfnCurrentEndThread = pfnEndThread;
	
	pThreadParms->args = args;

	if (pfnBeginThread) {
		unsigned threadid = 0;
		thread->handle = (SYS_ThreadHandle)
				((size_t) pfnBeginThread(NULL, 0, RunThreadViaBeginThreadEx,
										 pThreadParms, 0, &threadid));
	} else {
		DWORD threadid = 0;
		thread->handle = CreateThread(NULL, 0, RunThreadViaCreateThread, pThreadParms, 0, &threadid);
	}
	if (thread->handle == NULL) {
		SDL_SetError("Not enough resources to create thread");
		return(-1);
	}
	return(0);
}

void SDL_SYS_SetupThread(void)
{
	return;
}

Uint32 SDL_ThreadID(void)
{
	return((Uint32)GetCurrentThreadId());
}

void SDL_SYS_WaitThread(SDL_Thread *thread)
{
	WaitForSingleObject(thread->handle, INFINITE);
	CloseHandle(thread->handle);
}

void SDL_SYS_KillThread(SDL_Thread *thread)
{
	TerminateThread(thread->handle, FALSE);
}
