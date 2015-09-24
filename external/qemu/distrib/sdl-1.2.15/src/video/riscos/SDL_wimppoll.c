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


#include "SDL.h"
#include "SDL_syswm.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "SDL_riscosvideo.h"
#include "SDL_riscosevents_c.h"
#include "SDL_riscosmouse_c.h"
#include "../../timer/SDL_timer_c.h"

#include "memory.h"
#include "stdlib.h"
#include "ctype.h"

#include "kernel.h"
#include "swis.h"
#include "unixlib/os.h"

#if !SDL_THREADS_DISABLED
#include <pthread.h>
#endif

void WIMP_Poll(_THIS, int waitTime);
void WIMP_SetFocus(int win);

void WIMP_PlotSprite(_THIS, int x, int y);
void WIMP_ModeChanged(_THIS);
void WIMP_PaletteChanged(_THIS);


extern void WIMP_PollMouse(_THIS);
extern void RISCOS_PollKeyboard();

#if SDL_THREADS_DISABLED
extern void RISCOS_CheckTimer();
#else
extern int riscos_using_threads;
#endif

extern void WIMP_ReshowCursor(_THIS);
extern void WIMP_RestoreWimpCursor();

int hasFocus = 0;
int mouseInWindow = 0;
 
static int resizeOnOpen = 0;

void WIMP_PumpEvents(_THIS)
{
	WIMP_Poll(this, 0);
	if (hasFocus) RISCOS_PollKeyboard();
	if (mouseInWindow) WIMP_PollMouse(this);
#if SDL_THREADS_DISABLED
	if (SDL_timer_running) RISCOS_CheckTimer();
#endif
}


void WIMP_Poll(_THIS, int waitTime)
{
	_kernel_swi_regs regs;
	int message[64];
	unsigned int code;
	int pollMask = 0;
	int doPoll = 1;
	int sysEvent;
	int sdlWindow = this->hidden->window_handle;

    if (this->PumpEvents != WIMP_PumpEvents) return;

    if (waitTime > 0)
    {
		_kernel_swi(OS_ReadMonotonicTime, &regs, &regs);
		waitTime += regs.r[0];
    }

    while (doPoll)
    {
#if !SDL_THREADS_DISABLED
       
       if (riscos_using_threads) __pthread_stop_ticker();
#endif

        if (waitTime <= 0)
        {
        	regs.r[0] = pollMask; 
        	
        	if (waitTime < 0) regs.r[0] |= 1;
        	regs.r[1] = (int)message;
        	_kernel_swi(Wimp_Poll, &regs, &regs);
        } else
        {
        	regs.r[0] = pollMask;
        	regs.r[1] = (int)message;
        	regs.r[2] = waitTime;
        	_kernel_swi(Wimp_PollIdle, &regs, &regs);
        }

		
	sysEvent = 0;
        
        code = (unsigned int)regs.r[0];

	switch(code)
	{
        case 0:  
	   doPoll = 0;
	   break;

	case 1:     
           _kernel_swi(Wimp_RedrawWindow, &regs,&regs);
	   if (message[0] == sdlWindow)
	   {
                 while (regs.r[0])
                 {
           	    WIMP_PlotSprite(this, message[1], message[2]);
           	    _kernel_swi(Wimp_GetRectangle, &regs, &regs);
                 }
	   } else
	  {
	
        	while (regs.r[0])
        	{
                        _kernel_swi(Wimp_GetRectangle, &regs, &regs);
        	}
	  }
          break;
        	
		case 2:		
		   if ( resizeOnOpen && message[0] == sdlWindow)
		   {
		      
		      resizeOnOpen = 0;
		      message[3] = message[1] + (this->screen->w << this->hidden->xeig);
		      message[4] = message[2] + (this->screen->h << this->hidden->yeig);       
		   }
        	_kernel_swi(Wimp_OpenWindow, &regs, &regs);
       	    break;
        	
		case 3:		
			if (message[0] == sdlWindow)
			{
				SDL_PrivateQuit();
			} else
				sysEvent = 1;
        	doPoll = 0;
        	break;

		case 4: 
			if (message[0] == sdlWindow)
			{
				mouseInWindow = 0;
				
				 
				 WIMP_RestoreWimpCursor();
				 SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);
			} else
				sysEvent = 1;
			break;

		case 5: 
			if (message[0] == sdlWindow) 
			{
				mouseInWindow = 1;
				WIMP_ReshowCursor(this);
				SDL_PrivateAppActive(1, SDL_APPMOUSEFOCUS);
			} else sysEvent = 1;
			break;

		case 6:		
			if (hasFocus == 0)
			{
			   
			   
			   WIMP_SetFocus(message[3]);
			} else
				doPoll = 0; 
		   break;

		case 7: 
			
			sysEvent = 1;
			break;

		case 8: 
			doPoll = 0; 
			if (message[0] != sdlWindow) sysEvent = 1;
			break;

		case 11: 
			 hasFocus = 0;
			 if (message[0] == sdlWindow) SDL_PrivateAppActive(0, SDL_APPINPUTFOCUS);
			 else sysEvent = 1;
			 break;

		case 12: 
			 hasFocus = 1;
			 if (message[0] == sdlWindow) SDL_PrivateAppActive(1, SDL_APPINPUTFOCUS);
			 else sysEvent = 1;
			 break;
        	
		case 17:
		case 18:
			sysEvent = 1; 

			switch(message[4])
			{
			case 0: 
				
			   SDL_Quit();
        	   exit(0);
			   break;

			case 8: 
				SDL_PrivateQuit();
				break;

			case 0x400c1: 
				WIMP_ModeChanged(this);
				resizeOnOpen = 1;
				break;

			case 9:      
				WIMP_PaletteChanged(this);
				break;
			}
			break;

		default:
			
			sysEvent = 1;
			break;
		}

		if (sysEvent)
		{
	        SDL_SysWMmsg wmmsg;

			SDL_VERSION(&wmmsg.version);
			wmmsg.eventCode = code;
			SDL_memcpy(wmmsg.pollBlock, message, 64 * sizeof(int));

			
			if (SDL_PrivateSysWMEvent(&wmmsg)) doPoll = 0;
		}
#if !SDL_THREADS_DISABLED
		if (riscos_using_threads)
		{
		   if (riscos_using_threads) __pthread_start_ticker();
                   
		   pthread_yield();
		}
#endif
    }
}

void WIMP_SetFocus(int win)
{
	_kernel_swi_regs regs;

	regs.r[0] = win;
	regs.r[1] = -1; 
	regs.r[2] = 0;  
	regs.r[3] = 0;  
	regs.r[4] = 1 << 25; 
	regs.r[5] = 0;  

	_kernel_swi(Wimp_SetCaretPosition, &regs, &regs);
}

void RISCOS_BackgroundTasks(void)
{
	if (current_video && current_video->hidden->window_handle)
	{
		WIMP_Poll(current_video, 0);
	}
#if SDL_THREADS_DISABLED
	if (SDL_timer_running) RISCOS_CheckTimer();
#endif
}
