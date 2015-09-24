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

#ifndef _SDL_mutex_h
#define _SDL_mutex_h


#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SDL_MUTEX_TIMEDOUT	1

#define SDL_MUTEX_MAXWAIT	(~(Uint32)0)


 

struct SDL_mutex;
typedef struct SDL_mutex SDL_mutex;

extern DECLSPEC SDL_mutex * SDLCALL SDL_CreateMutex(void);

#define SDL_LockMutex(m)	SDL_mutexP(m)
extern DECLSPEC int SDLCALL SDL_mutexP(SDL_mutex *mutex);

#define SDL_UnlockMutex(m)	SDL_mutexV(m)
extern DECLSPEC int SDLCALL SDL_mutexV(SDL_mutex *mutex);

extern DECLSPEC void SDLCALL SDL_DestroyMutex(SDL_mutex *mutex);


 

struct SDL_semaphore;
typedef struct SDL_semaphore SDL_sem;

extern DECLSPEC SDL_sem * SDLCALL SDL_CreateSemaphore(Uint32 initial_value);

extern DECLSPEC void SDLCALL SDL_DestroySemaphore(SDL_sem *sem);

extern DECLSPEC int SDLCALL SDL_SemWait(SDL_sem *sem);

extern DECLSPEC int SDLCALL SDL_SemTryWait(SDL_sem *sem);

extern DECLSPEC int SDLCALL SDL_SemWaitTimeout(SDL_sem *sem, Uint32 ms);

extern DECLSPEC int SDLCALL SDL_SemPost(SDL_sem *sem);

extern DECLSPEC Uint32 SDLCALL SDL_SemValue(SDL_sem *sem);


 

struct SDL_cond;
typedef struct SDL_cond SDL_cond;

extern DECLSPEC SDL_cond * SDLCALL SDL_CreateCond(void);

extern DECLSPEC void SDLCALL SDL_DestroyCond(SDL_cond *cond);

extern DECLSPEC int SDLCALL SDL_CondSignal(SDL_cond *cond);

extern DECLSPEC int SDLCALL SDL_CondBroadcast(SDL_cond *cond);

extern DECLSPEC int SDLCALL SDL_CondWait(SDL_cond *cond, SDL_mutex *mut);

extern DECLSPEC int SDLCALL SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, Uint32 ms);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 

