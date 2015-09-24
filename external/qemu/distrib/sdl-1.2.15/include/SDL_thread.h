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

#ifndef _SDL_thread_h
#define _SDL_thread_h


#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "SDL_mutex.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

struct SDL_Thread;
typedef struct SDL_Thread SDL_Thread;

#if ((defined(__WIN32__) && !defined(HAVE_LIBC)) || defined(__OS2__)) &&  !defined(__SYMBIAN32__)
#define SDL_PASSED_BEGINTHREAD_ENDTHREAD
#ifndef _WIN32_WCE
#include <process.h> 
#endif

#ifdef __OS2__
typedef int (*pfnSDL_CurrentBeginThread)(void (*func)(void *), void *, unsigned, void *arg); 
typedef void (*pfnSDL_CurrentEndThread)(void);
#else
typedef uintptr_t (__cdecl *pfnSDL_CurrentBeginThread) (void *, unsigned,
        unsigned (__stdcall *func)(void *), void *arg, 
        unsigned, unsigned *threadID);
typedef void (__cdecl *pfnSDL_CurrentEndThread)(unsigned code);
#endif

extern DECLSPEC SDL_Thread * SDLCALL SDL_CreateThread(int (SDLCALL *fn)(void *), void *data, pfnSDL_CurrentBeginThread pfnBeginThread, pfnSDL_CurrentEndThread pfnEndThread);

#ifdef __OS2__
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, data, _beginthread, _endthread)
#elif defined(_WIN32_WCE)
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, data, NULL, NULL)
#else
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, data, _beginthreadex, _endthreadex)
#endif
#else
extern DECLSPEC SDL_Thread * SDLCALL SDL_CreateThread(int (SDLCALL *fn)(void *), void *data);
#endif

extern DECLSPEC Uint32 SDLCALL SDL_ThreadID(void);

extern DECLSPEC Uint32 SDLCALL SDL_GetThreadID(SDL_Thread *thread);

extern DECLSPEC void SDLCALL SDL_WaitThread(SDL_Thread *thread, int *status);

extern DECLSPEC void SDLCALL SDL_KillThread(SDL_Thread *thread);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 
