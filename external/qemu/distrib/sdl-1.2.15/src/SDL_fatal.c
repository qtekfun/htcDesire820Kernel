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


#ifdef HAVE_SIGNAL_H

#include <signal.h>

#include "SDL.h"
#include "SDL_fatal.h"


static void SDL_Parachute(int sig)
{
	signal(sig, SIG_DFL);
	SDL_Quit();
	raise(sig);
}

static int SDL_fatal_signals[] = {
	SIGSEGV,
#ifdef SIGBUS
	SIGBUS,
#endif
#ifdef SIGFPE
	SIGFPE,
#endif
#ifdef SIGQUIT
	SIGQUIT,
#endif
	0
};

void SDL_InstallParachute(void)
{
	
	int i;
#ifdef HAVE_SIGACTION
	struct sigaction action;

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		sigaction(SDL_fatal_signals[i], NULL, &action);
		if ( action.sa_handler == SIG_DFL ) {
			action.sa_handler = SDL_Parachute;
			sigaction(SDL_fatal_signals[i], &action, NULL);
		}
	}
#ifdef SIGALRM
	
	sigaction(SIGALRM, NULL, &action);
	if ( action.sa_handler == SIG_DFL ) {
		action.sa_handler = SIG_IGN;
		sigaction(SIGALRM, &action, NULL);
	}
#endif
#else
	void (*ohandler)(int);

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SDL_Parachute);
		if ( ohandler != SIG_DFL ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#endif 
	return;
}

void SDL_UninstallParachute(void)
{
	
	int i;
#ifdef HAVE_SIGACTION
	struct sigaction action;

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		sigaction(SDL_fatal_signals[i], NULL, &action);
		if ( action.sa_handler == SDL_Parachute ) {
			action.sa_handler = SIG_DFL;
			sigaction(SDL_fatal_signals[i], &action, NULL);
		}
	}
#else
	void (*ohandler)(int);

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SIG_DFL);
		if ( ohandler != SDL_Parachute ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#endif 
}

#else


void SDL_InstallParachute(void)
{
	return;
}

void SDL_UninstallParachute(void)
{
	return;
}

#endif 
