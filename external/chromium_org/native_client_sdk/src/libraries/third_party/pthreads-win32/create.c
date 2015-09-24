/*
 * create.c
 *
 * Description:
 * This translation unit implements routines associated with spawning a new
 * thread.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 * 
 *      Contact Email: rpj@callisto.canberra.edu.au
 * 
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "pthread.h"
#include "implement.h"
#if ! defined(_UWIN) && ! defined(WINCE)
#include <process.h>
#endif

int
pthread_create (pthread_t * tid,
		const pthread_attr_t * attr,
		void *(PTW32_CDECL *start) (void *), void *arg)
{
  pthread_t thread;
  ptw32_thread_t * tp;
  register pthread_attr_t a;
  HANDLE threadH = 0;
  int result = EAGAIN;
  int run = PTW32_TRUE;
  ThreadParms *parms = NULL;
  unsigned int stackSize;
  int priority;
  pthread_t self;

  tid->x = 0;

  if (attr != NULL)
    {
      a = *attr;
    }
  else
    {
      a = NULL;
    }

  if ((thread = ptw32_new ()).p == NULL)
    {
      goto FAIL0;
    }

  tp = (ptw32_thread_t *) thread.p;

  priority = tp->sched_priority;

  if ((parms = (ThreadParms *) malloc (sizeof (*parms))) == NULL)
    {
      goto FAIL0;
    }

  parms->tid = thread;
  parms->start = start;
  parms->arg = arg;

#if defined(HAVE_SIGSET_T)

  self = pthread_self();
  tp->sigmask = ((ptw32_thread_t *)self.p)->sigmask;

#endif 


  if (a != NULL)
    {
      stackSize = (unsigned int)a->stacksize;
      tp->detachState = a->detachstate;
      priority = a->param.sched_priority;

#if (THREAD_PRIORITY_LOWEST > THREAD_PRIORITY_NORMAL)
      
#else
      


      if (PTHREAD_INHERIT_SCHED == a->inheritsched)
	{
#if ! defined(HAVE_SIGSET_T)
	  self = pthread_self ();
#endif
	  priority = ((ptw32_thread_t *) self.p)->sched_priority;
	}

#endif

    }
  else
    {
      stackSize = PTHREAD_STACK_MIN;
    }

  tp->state = run ? PThreadStateInitial : PThreadStateSuspended;

  tp->keys = NULL;


#if ! (defined (__MINGW64__) || defined(__MINGW32__)) || defined (__MSVCRT__) || defined (__DMC__) 

  tp->threadH =
    threadH =
    (HANDLE) _beginthreadex ((void *) NULL,	
			     stackSize,		
			     ptw32_threadStart,
			     parms,
			     (unsigned)
			     CREATE_SUSPENDED,
			     (unsigned *) &(tp->thread));

  if (threadH != 0)
    {
      if (a != NULL)
	{
	  (void) ptw32_setthreadpriority (thread, SCHED_OTHER, priority);
	}

      if (run)
	{
	  ResumeThread (threadH);
	}
    }

#else

  {
    ptw32_mcs_local_node_t stateLock;

    ptw32_mcs_lock_acquire(&tp->stateLock, &stateLock);

    tp->threadH =
      threadH =
      (HANDLE) _beginthread (ptw32_threadStart, stackSize,	
			     parms);

    if (threadH == (HANDLE) - 1L)
      {
        tp->threadH = threadH = 0;
      }
    else
      {
        if (!run)
	  {
	    SuspendThread (threadH);
	  }
  
        if (a != NULL)
	  {
	    (void) ptw32_setthreadpriority (thread, SCHED_OTHER, priority);
	  }
      }

    ptw32_mcs_lock_release (&stateLock);
  }
#endif

  result = (threadH != 0) ? 0 : EAGAIN;



FAIL0:
  if (result != 0)
    {

      ptw32_threadDestroy (thread);
      tp = NULL;

      if (parms != NULL)
	{
	  free (parms);
	}
    }
  else
    {
      *tid = thread;
    }

#if defined(_UWIN)
  if (result == 0)
    pthread_count++;
#endif
  return (result);

}				
