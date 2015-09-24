/*
 * ptw32_throw.c
 *
 * Description:
 * This translation unit implements routines which are private to
 * the implementation and may be used throughout it.
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

#if defined(__CLEANUP_C)
# include <setjmp.h>
#endif

#if defined(_MSC_VER)
#pragma warning(disable:4290)
#endif
void
ptw32_throw (DWORD exception)
#if defined(__CLEANUP_CXX)
  throw(ptw32_exception_cancel,ptw32_exception_exit)
#endif
{
  ptw32_thread_t * sp = (ptw32_thread_t *) pthread_getspecific (ptw32_selfThreadKey);

#if defined(__CLEANUP_SEH)
  DWORD exceptionInformation[3];
#endif

  sp->state = PThreadStateExiting;

  if (exception != PTW32_EPS_CANCEL && exception != PTW32_EPS_EXIT)
    {
      
      exit (1);
    }

  if (NULL == sp || sp->implicit)
    {
#if ! (defined(__MINGW64__) || defined(__MINGW32__)) || defined (__MSVCRT__) || defined (__DMC__)
      unsigned exitCode = 0;

      switch (exception)
	{
	case PTW32_EPS_CANCEL:
	  exitCode = (unsigned)(size_t) PTHREAD_CANCELED;
	  break;
	case PTW32_EPS_EXIT:
	  if (NULL != sp)
	    {
	      exitCode = (unsigned)(size_t) sp->exitStatus;
	    }
	  break;
	}
#endif

#if defined(PTW32_STATIC_LIB)

      pthread_win32_thread_detach_np ();

#endif

#if ! (defined(__MINGW64__) || defined(__MINGW32__)) || defined (__MSVCRT__) || defined (__DMC__)
      _endthreadex (exitCode);
#else
      _endthread ();
#endif

    }

#if defined(__CLEANUP_SEH)


  exceptionInformation[0] = (DWORD) (exception);
  exceptionInformation[1] = (DWORD) (0);
  exceptionInformation[2] = (DWORD) (0);

  RaiseException (EXCEPTION_PTW32_SERVICES, 0, 3, exceptionInformation);

#else 

#if defined(__CLEANUP_C)

  ptw32_pop_cleanup_all (1);
  longjmp (sp->start_mark, exception);

#else 

#if defined(__CLEANUP_CXX)

  switch (exception)
    {
    case PTW32_EPS_CANCEL:
      throw ptw32_exception_cancel ();
      break;
    case PTW32_EPS_EXIT:
      throw ptw32_exception_exit ();
      break;
    }

#else

#error ERROR [__FILE__, line __LINE__]: Cleanup type undefined.

#endif 

#endif 

#endif 

  
}


void
ptw32_pop_cleanup_all (int execute)
{
  while (NULL != ptw32_pop_cleanup (execute))
    {
    }
}


DWORD
ptw32_get_exception_services_code (void)
{
#if defined(__CLEANUP_SEH)

  return EXCEPTION_PTW32_SERVICES;

#else

  return (DWORD)0;

#endif
}
