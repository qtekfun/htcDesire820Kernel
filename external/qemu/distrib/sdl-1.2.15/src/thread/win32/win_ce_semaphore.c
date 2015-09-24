/* win_ce_semaphore.c

   Copyright (c) 1998, Johnson M. Hart
   (with corrections 2001 by Rainer Loritz)
   Permission is granted for any and all use providing that this
   copyright is properly acknowledged.
   There are no assurances of suitability for any use whatsoever.

   WINDOWS CE: There is a collection of Windows CE functions to simulate
   semaphores using only a mutex and an event. As Windows CE events cannot
   be named, these simulated semaphores cannot be named either.

   Implementation notes:
   1. All required internal data structures are allocated on the process's heap.
   2. Where appropriate, a new error code is returned (see the header
      file), or, if the error is a Win32 error, that code is unchanged.
   3. Notice the new handle type "SYNCHHANDLE" that has handles, counters,
      and other information. This structure will grow as new objects are added
      to this set; some members are specific to only one or two of the objects.
   4. Mutexes are used for critical sections. These could be replaced with
      CRITICAL_SECTION objects but then this would give up the time out
      capability.
   5. The implementation shows several interesting aspects of synchronization, some
      of which are specific to Win32 and some of which are general. These are pointed
      out in the comments as appropriate.
   6. The wait function emulates WaitForSingleObject only. An emulation of
      WaitForMultipleObjects is much harder to implement outside the kernel,
      and it is not clear how to handle a mixture of WCE semaphores and normal
      events and mutexes. */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "win_ce_semaphore.h"

static SYNCHHANDLE CleanUp (SYNCHHANDLE hSynch, DWORD Flags);

SYNCHHANDLE CreateSemaphoreCE (

   LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,  
      LONG lInitialCount,   
      LONG lMaximumCount,   
      LPCTSTR lpName )


{
   SYNCHHANDLE hSynch = NULL, result = NULL;

   __try
	{
      if (lInitialCount > lMaximumCount || lMaximumCount < 0 || lInitialCount < 0) 
	  {
              
         SetLastError (SYNCH_ERROR);
         __leave;
      }

      hSynch = HeapAlloc (GetProcessHeap(), HEAP_ZERO_MEMORY, SYNCH_HANDLE_SIZE);
      if (hSynch == NULL) __leave;

      hSynch->MaxCount = lMaximumCount;
      hSynch->CurCount = lInitialCount;
      hSynch->lpName = lpName;

      hSynch->hMutex = CreateMutex (lpSemaphoreAttributes, FALSE, NULL);

      WaitForSingleObject (hSynch->hMutex, INFINITE);
      hSynch->hEvent = CreateEvent (lpSemaphoreAttributes, FALSE, 
              lInitialCount > 0, NULL);
      ReleaseMutex (hSynch->hMutex);
      hSynch->hSemph = NULL;
   }
   __finally
   {
      result=CleanUp(hSynch, 6 );
   }

   return result;
}

BOOL ReleaseSemaphoreCE (SYNCHHANDLE hSemCE, LONG cReleaseCount, LPLONG lpPreviousCount)
{
   BOOL Result = TRUE;


   __try 
   {
      WaitForSingleObject (hSemCE->hMutex, INFINITE);
	  	
	  if (lpPreviousCount!=NULL)
		 *lpPreviousCount = hSemCE->CurCount;
      if (hSemCE->CurCount + cReleaseCount > hSemCE->MaxCount || cReleaseCount <= 0)
	  {
         SetLastError (SYNCH_ERROR);
         Result = FALSE;
         __leave;
      }
      hSemCE->CurCount += cReleaseCount;


      SetEvent (hSemCE->hEvent);
   }
   __finally
   {
      ReleaseMutex (hSemCE->hMutex);
   }

   return Result;
}

DWORD WaitForSemaphoreCE (SYNCHHANDLE hSemCE, DWORD dwMilliseconds)
   
{
   DWORD WaitResult;

   WaitResult = WaitForSingleObject (hSemCE->hMutex, dwMilliseconds);
   if (WaitResult != WAIT_OBJECT_0 && WaitResult != WAIT_ABANDONED_0) return WaitResult;
   while (hSemCE->CurCount <= 0) 
   { 


      ReleaseMutex (hSemCE->hMutex);


      WaitResult = WaitForSingleObject (hSemCE->hEvent, dwMilliseconds);
      if (WaitResult != WAIT_OBJECT_0) return WaitResult;


      WaitResult = WaitForSingleObject (hSemCE->hMutex, dwMilliseconds);
      if (WaitResult != WAIT_OBJECT_0 && WaitResult != WAIT_ABANDONED_0) return WaitResult;

   }
   

   hSemCE->CurCount--;

   if (hSemCE->CurCount > 0) SetEvent (hSemCE->hEvent);
   ReleaseMutex (hSemCE->hMutex);
   return WaitResult;
}

BOOL CloseSynchHandle (SYNCHHANDLE hSynch)
{
   BOOL Result = TRUE;
   if (hSynch->hEvent != NULL) Result = Result && CloseHandle (hSynch->hEvent);
   if (hSynch->hMutex != NULL) Result = Result && CloseHandle (hSynch->hMutex);
   if (hSynch->hSemph != NULL) Result = Result && CloseHandle (hSynch->hSemph);
   HeapFree (GetProcessHeap (), 0, hSynch);
   return (Result);
}

static SYNCHHANDLE CleanUp (SYNCHHANDLE hSynch, DWORD Flags)
{ 

   BOOL ok = TRUE;

   if (hSynch == NULL) return NULL;
   if ((Flags & 4) == 1 && (hSynch->hEvent == NULL)) ok = FALSE;
   if ((Flags & 2) == 1 && (hSynch->hMutex == NULL)) ok = FALSE;
   if ((Flags & 1) == 1 && (hSynch->hEvent == NULL)) ok = FALSE;
   if (!ok) 
   {
      CloseSynchHandle (hSynch);
      return NULL;
   }
   
   return hSynch;
}
