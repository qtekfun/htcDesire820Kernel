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


#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_sysevents.h"
#include "SDL_events_c.h"
#include "../timer/SDL_timer_c.h"
#if !SDL_JOYSTICK_DISABLED
#include "../joystick/SDL_joystick_c.h"
#endif

SDL_EventFilter SDL_EventOK = NULL;
Uint8 SDL_ProcessEvents[SDL_NUMEVENTS];
static Uint32 SDL_eventstate = 0;

#define MAXEVENTS	128
static struct {
	SDL_mutex *lock;
	int active;
	int head;
	int tail;
	SDL_Event event[MAXEVENTS];
	int wmmsg_next;
	struct SDL_SysWMmsg wmmsg[MAXEVENTS];
} SDL_EventQ;

static struct {
	SDL_mutex *lock;
	int safe;
} SDL_EventLock;

static SDL_Thread *SDL_EventThread = NULL;	
static Uint32 event_thread;			

void SDL_Lock_EventThread(void)
{
	if ( SDL_EventThread && (SDL_ThreadID() != event_thread) ) {
		
		SDL_mutexP(SDL_EventLock.lock);
		while ( ! SDL_EventLock.safe ) {
			SDL_Delay(1);
		}
	}
}
void SDL_Unlock_EventThread(void)
{
	if ( SDL_EventThread && (SDL_ThreadID() != event_thread) ) {
		SDL_mutexV(SDL_EventLock.lock);
	}
}

#ifdef __OS2__
#define INCL_DOSPROCESS
#include <os2.h>
#include <time.h>
#endif

static int SDLCALL SDL_GobbleEvents(void *unused)
{
	event_thread = SDL_ThreadID();

#ifdef __OS2__
#ifdef USE_DOSSETPRIORITY
	
	DosSetPriority(PRTYS_THREAD, PRTYC_REGULAR, +16, 0);
#endif
#endif

	while ( SDL_EventQ.active ) {
		SDL_VideoDevice *video = current_video;
		SDL_VideoDevice *this  = current_video;

		
		if ( video ) {
			video->PumpEvents(this);
		}

		
		SDL_CheckKeyRepeat();

#if !SDL_JOYSTICK_DISABLED
		
		if ( SDL_numjoysticks && (SDL_eventstate & SDL_JOYEVENTMASK) ) {
			SDL_JoystickUpdate();
		}
#endif

		
		SDL_EventLock.safe = 1;
		if ( SDL_timer_running ) {
			SDL_ThreadedTimerCheck();
		}
		SDL_Delay(1);

		SDL_mutexP(SDL_EventLock.lock);
		SDL_EventLock.safe = 0;
		SDL_mutexV(SDL_EventLock.lock);
	}
	SDL_SetTimerThreaded(0);
	event_thread = 0;
	return(0);
}

static int SDL_StartEventThread(Uint32 flags)
{
	
	SDL_EventThread = NULL;
	SDL_memset(&SDL_EventLock, 0, sizeof(SDL_EventLock));

	
#if !SDL_THREADS_DISABLED
	SDL_EventQ.lock = SDL_CreateMutex();
	if ( SDL_EventQ.lock == NULL ) {
#ifdef __MACOS__ 
		;
#else
		return(-1);
#endif
	}
#endif 
	SDL_EventQ.active = 1;

	if ( (flags&SDL_INIT_EVENTTHREAD) == SDL_INIT_EVENTTHREAD ) {
		SDL_EventLock.lock = SDL_CreateMutex();
		if ( SDL_EventLock.lock == NULL ) {
			return(-1);
		}
		SDL_EventLock.safe = 0;

		
		SDL_SetTimerThreaded(2);
#if (defined(__WIN32__) && !defined(_WIN32_WCE)) && !defined(HAVE_LIBC) && !defined(__SYMBIAN32__)
#undef SDL_CreateThread
		SDL_EventThread = SDL_CreateThread(SDL_GobbleEvents, NULL, NULL, NULL);
#else
		SDL_EventThread = SDL_CreateThread(SDL_GobbleEvents, NULL);
#endif
		if ( SDL_EventThread == NULL ) {
			return(-1);
		}
	} else {
		event_thread = 0;
	}
	return(0);
}

static void SDL_StopEventThread(void)
{
	SDL_EventQ.active = 0;
	if ( SDL_EventThread ) {
		SDL_WaitThread(SDL_EventThread, NULL);
		SDL_EventThread = NULL;
		SDL_DestroyMutex(SDL_EventLock.lock);
		SDL_EventLock.lock = NULL;
	}
#ifndef IPOD
	SDL_DestroyMutex(SDL_EventQ.lock);
	SDL_EventQ.lock = NULL;
#endif
}

Uint32 SDL_EventThreadID(void)
{
	return(event_thread);
}


void SDL_StopEventLoop(void)
{
	
	SDL_StopEventThread();

	
	SDL_AppActiveQuit();
	SDL_KeyboardQuit();
	SDL_MouseQuit();
	SDL_QuitQuit();

	
	SDL_EventQ.head = 0;
	SDL_EventQ.tail = 0;
	SDL_EventQ.wmmsg_next = 0;
}

int SDL_StartEventLoop(Uint32 flags)
{
	int retcode;

	
	SDL_EventThread = NULL;
	SDL_EventQ.lock = NULL;
	SDL_StopEventLoop();

	
	SDL_EventOK = NULL;
	SDL_memset(SDL_ProcessEvents,SDL_ENABLE,sizeof(SDL_ProcessEvents));
	SDL_eventstate = ~0;
	
	SDL_eventstate &= ~(0x00000001 << SDL_SYSWMEVENT);
	SDL_ProcessEvents[SDL_SYSWMEVENT] = SDL_IGNORE;

	
	retcode = 0;
	retcode += SDL_AppActiveInit();
	retcode += SDL_KeyboardInit();
	retcode += SDL_MouseInit();
	retcode += SDL_QuitInit();
	if ( retcode < 0 ) {
		
		return(-1);
	}

	
	if ( SDL_StartEventThread(flags) < 0 ) {
		SDL_StopEventLoop();
		return(-1);
	}
	return(0);
}


static int SDL_AddEvent(SDL_Event *event)
{
	int tail, added;

	tail = (SDL_EventQ.tail+1)%MAXEVENTS;
	if ( tail == SDL_EventQ.head ) {
		
		added = 0;
	} else {
		SDL_EventQ.event[SDL_EventQ.tail] = *event;
		if (event->type == SDL_SYSWMEVENT) {
			
			int next = SDL_EventQ.wmmsg_next;
			SDL_EventQ.wmmsg[next] = *event->syswm.msg;
		        SDL_EventQ.event[SDL_EventQ.tail].syswm.msg =
						&SDL_EventQ.wmmsg[next];
			SDL_EventQ.wmmsg_next = (next+1)%MAXEVENTS;
		}
		SDL_EventQ.tail = tail;
		added = 1;
	}
	return(added);
}

static int SDL_CutEvent(int spot)
{
	if ( spot == SDL_EventQ.head ) {
		SDL_EventQ.head = (SDL_EventQ.head+1)%MAXEVENTS;
		return(SDL_EventQ.head);
	} else
	if ( (spot+1)%MAXEVENTS == SDL_EventQ.tail ) {
		SDL_EventQ.tail = spot;
		return(SDL_EventQ.tail);
	} else
	
	{
		int here, next;

		
		if ( --SDL_EventQ.tail < 0 ) {
			SDL_EventQ.tail = MAXEVENTS-1;
		}
		for ( here=spot; here != SDL_EventQ.tail; here = next ) {
			next = (here+1)%MAXEVENTS;
			SDL_EventQ.event[here] = SDL_EventQ.event[next];
		}
		return(spot);
	}
	
}

int SDL_PeepEvents(SDL_Event *events, int numevents, SDL_eventaction action,
								Uint32 mask)
{
	int i, used;

	
	if ( ! SDL_EventQ.active ) {
		return(-1);
	}
	
	used = 0;
	if ( SDL_mutexP(SDL_EventQ.lock) == 0 ) {
		if ( action == SDL_ADDEVENT ) {
			for ( i=0; i<numevents; ++i ) {
				used += SDL_AddEvent(&events[i]);
			}
		} else {
			SDL_Event tmpevent;
			int spot;

			
			if ( events == NULL ) {
				action = SDL_PEEKEVENT;
				numevents = 1;
				events = &tmpevent;
			}
			spot = SDL_EventQ.head;
			while ((used < numevents)&&(spot != SDL_EventQ.tail)) {
				if ( mask & SDL_EVENTMASK(SDL_EventQ.event[spot].type) ) {
					events[used++] = SDL_EventQ.event[spot];
					if ( action == SDL_GETEVENT ) {
						spot = SDL_CutEvent(spot);
					} else {
						spot = (spot+1)%MAXEVENTS;
					}
				} else {
					spot = (spot+1)%MAXEVENTS;
				}
			}
		}
		SDL_mutexV(SDL_EventQ.lock);
	} else {
		SDL_SetError("Couldn't lock event queue");
		used = -1;
	}
	return(used);
}

void SDL_PumpEvents(void)
{
	if ( !SDL_EventThread ) {
		SDL_VideoDevice *video = current_video;
		SDL_VideoDevice *this  = current_video;

		
		if ( video ) {
			video->PumpEvents(this);
		}

		
		SDL_CheckKeyRepeat();

#if !SDL_JOYSTICK_DISABLED
		
		if ( SDL_numjoysticks && (SDL_eventstate & SDL_JOYEVENTMASK) ) {
			SDL_JoystickUpdate();
		}
#endif
	}
}


int SDL_PollEvent (SDL_Event *event)
{
	SDL_PumpEvents();

	
	if ( SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_ALLEVENTS) <= 0 )
		return 0;
	return 1;
}

int SDL_WaitEvent (SDL_Event *event)
{
	while ( 1 ) {
		SDL_PumpEvents();
		switch(SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_ALLEVENTS)) {
		    case -1: return 0;
		    case 1: return 1;
		    case 0: SDL_Delay(10);
		}
	}
}

int SDL_PushEvent(SDL_Event *event)
{
	if ( SDL_PeepEvents(event, 1, SDL_ADDEVENT, 0) <= 0 )
		return -1;
	return 0;
}

void SDL_SetEventFilter (SDL_EventFilter filter)
{
	SDL_Event bitbucket;

	
	SDL_EventOK = filter;
	while ( SDL_PollEvent(&bitbucket) > 0 )
		;
}

SDL_EventFilter SDL_GetEventFilter(void)
{
	return(SDL_EventOK);
}

Uint8 SDL_EventState (Uint8 type, int state)
{
	SDL_Event bitbucket;
	Uint8 current_state;

	
	if ( type == 0xFF ) {
		current_state = SDL_IGNORE;
		for ( type=0; type<SDL_NUMEVENTS; ++type ) {
			if ( SDL_ProcessEvents[type] != SDL_IGNORE ) {
				current_state = SDL_ENABLE;
			}
			SDL_ProcessEvents[type] = state;
			if ( state == SDL_ENABLE ) {
				SDL_eventstate |= (0x00000001 << (type));
			} else {
				SDL_eventstate &= ~(0x00000001 << (type));
			}
		}
		while ( SDL_PollEvent(&bitbucket) > 0 )
			;
		return(current_state);
	}

	
	current_state = SDL_ProcessEvents[type];
	switch (state) {
		case SDL_IGNORE:
		case SDL_ENABLE:
			
			SDL_ProcessEvents[type] = state;
			if ( state == SDL_ENABLE ) {
				SDL_eventstate |= (0x00000001 << (type));
			} else {
				SDL_eventstate &= ~(0x00000001 << (type));
			}
			while ( SDL_PollEvent(&bitbucket) > 0 )
				;
			break;
		default:
			
			break;
	}
	return(current_state);
}

int SDL_PrivateSysWMEvent(SDL_SysWMmsg *message)
{
	int posted;

	posted = 0;
	if ( SDL_ProcessEvents[SDL_SYSWMEVENT] == SDL_ENABLE ) {
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = SDL_SYSWMEVENT;
		event.syswm.msg = message;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
	
	return(posted);
}
