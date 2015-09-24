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

#ifdef SDL_TIMER_OS2

#define INCL_DOSMISC
#define INCL_DOSERRORS
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME
#define INCL_DOSPROCESS
#define INCL_DOSPROFILE
#define INCL_DOSEXCEPTIONS
#include <os2.h>

#include "SDL_thread.h"
#include "SDL_timer.h"
#include "../SDL_timer_c.h"


#define TIME_WRAP_VALUE (~(DWORD)0)

static long long hires_start_ticks;
static ULONG hires_ticks_per_second;

void SDL_StartTicks(void)
{
        DosTmrQueryFreq(&hires_ticks_per_second);
        DosTmrQueryTime((PQWORD)&hires_start_ticks);
}

DECLSPEC Uint32 SDLCALL SDL_GetTicks(void)
{
        long long hires_now;
        ULONG ticks = ticks;

        DosTmrQueryTime((PQWORD)&hires_now);
        
        _asm {
           push edx
           push eax
           mov eax, dword ptr hires_now
           mov edx, dword ptr hires_now+4
           sub eax, dword ptr hires_start_ticks
           sbb edx, dword ptr hires_start_ticks+4
           mov ebx,1000
           mov ecx,edx
           mul ebx
           push eax
           push edx
           mov eax,ecx
           mul ebx
           pop eax
           add edx,eax
           pop eax
           mov ebx, dword ptr hires_ticks_per_second
           div ebx
           mov dword ptr ticks, eax
           pop edx
           pop eax
        }

        return ticks;

}

DECLSPEC void SDLCALL SDL_Delay(Uint32 ms)
{
  HEV     hevEvent1     = 0;   
  HTIMER  htimerEvent1  = 0;   
  APIRET  rc            = NO_ERROR;  
  int ret = 1;
  ULONG priority = 0, nesting;   
  PPIB pib;
  PTIB tib;
  char *e = NULL;
  APIRET badrc;
  int switch_priority = 50;

  DosCreateEventSem(NULL,      
                    &hevEvent1,  
                    DC_SEM_SHARED, 
                    FALSE);      

  if (ms >= switch_priority)
    switch_priority = 0;
  if (switch_priority)
  {
    if (DosGetInfoBlocks(&tib, &pib)!=NO_ERROR)
      switch_priority = 0;
    else
    {
      priority = (tib->tib_ptib2->tib2_ulpri);
      if ((priority & 0xFF00) == 0x0300) 
        switch_priority = 0;
 
 
      if (DosEnterMustComplete(&nesting))
        switch_priority = 0;
      else
        DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
    }
  }

  if ((badrc = DosAsyncTimer(ms,
        (HSEM) hevEvent1, 
        &htimerEvent1))) 
    e = "DosAsyncTimer";

  if (switch_priority && tib->tib_ptib2->tib2_ulpri == 0x0300)
  {
 
  
    if (!(rc = DosSetPriority(PRTYS_THREAD, (priority>>8) & 0xFF, 0, 0)))
      rc = DosSetPriority(PRTYS_THREAD, 0, priority & 0xFF, 0);
  }
  if (switch_priority)
    rc = DosExitMustComplete(&nesting); 

  if (!e)     
    badrc = DosWaitEventSem(hevEvent1, SEM_INDEFINITE_WAIT);

  if (e) ;    
  else if (badrc == ERROR_INTERRUPT)
    ret = 0;
  else if (badrc)
    e = "DosWaitEventSem";
  if ((rc = DosCloseEventSem(hevEvent1)) && !e) { 
    e = "DosCloseEventSem";
    badrc = rc;
  }
  if (e)
  {
    SDL_SetError("[SDL_Delay] : Had error in %s(), rc is 0x%x\n", e, badrc);
  }
}

static int timer_alive = 0;
static SDL_Thread *timer = NULL;

static int SDLCALL RunTimer(void *unused)
{
        DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
        while ( timer_alive ) {
                if ( SDL_timer_running ) {
                        SDL_ThreadedTimerCheck();
                }
                SDL_Delay(10);
        }
        return(0);
}

int SDL_SYS_TimerInit(void)
{
        timer_alive = 1;
        timer = SDL_CreateThread(RunTimer, NULL);
        if ( timer == NULL )
                return(-1);
        return(SDL_SetTimerThreaded(1));
}

void SDL_SYS_TimerQuit(void)
{
        timer_alive = 0;
        if ( timer ) {
                SDL_WaitThread(timer, NULL);
                timer = NULL;
        }
}

int SDL_SYS_StartTimer(void)
{
        SDL_SetError("Internal logic error: OS/2 uses threaded timer");
        return(-1);
}

void SDL_SYS_StopTimer(void)
{
        return;
}

#endif 
