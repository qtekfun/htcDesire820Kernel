/* 
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape Portable Runtime (NSPR).
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1998-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef prtrace_h___
#define prtrace_h___

#include "prtypes.h"
#include "prthread.h"
#include "prtime.h"

PR_BEGIN_EXTERN_C

typedef void *  PRTraceHandle;

typedef struct PRTraceEntry
{
    PRThread        *thread;        
    PRTraceHandle   handle;         
    PRTime          time;           
    PRUint32        userData[8];    
} PRTraceEntry;

typedef enum PRTraceOption
{
    PRTraceBufSize,
    PRTraceEnable,              
    PRTraceDisable,
    PRTraceSuspend,
    PRTraceResume,
    PRTraceSuspendRecording,
    PRTraceResumeRecording,
    PRTraceLockHandles,
    PRTraceUnLockHandles,
    PRTraceStopRecording
} PRTraceOption;

#define PR_DEFINE_TRACE(name) PRTraceHandle name

#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_INIT_TRACE_HANDLE(handle,value)\
    (handle) = (PRCounterHandle)(value)
#else
#define PR_INIT_TRACE_HANDLE(handle,value)
#endif


#define PRTRACE_NAME_MAX 31
#define PRTRACE_DESC_MAX 255

#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_CREATE_TRACE(handle,qName,rName,description)\
    (handle) = PR_CreateTrace((qName),(rName),(description))
#else
#define PR_CREATE_TRACE(handle,qName,rName,description)
#endif

NSPR_API(PRTraceHandle)
	PR_CreateTrace( 
    	const char *qName,          
	    const char *rName,          
	    const char *description     
);


#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_DESTROY_TRACE(handle)\
    PR_DestroyTrace((handle))
#else
#define PR_DESTROY_TRACE(handle)
#endif

NSPR_API(void) 
	PR_DestroyTrace( 
		PRTraceHandle handle    
);


#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_TRACE(handle,ud0,ud1,ud2,ud3,ud4,ud5,ud6,ud7)\
    PR_Trace((handle),(ud0),(ud1),(ud2),(ud3),(ud4),(ud5),(ud6),(ud7))
#else
#define PR_TRACE(handle,ud0,ud1,ud2,ud3,ud4,ud5,ud6,ud7)
#endif

NSPR_API(void) 
	PR_Trace( 
    	PRTraceHandle handle,       
	    PRUint32    userData0,      
	    PRUint32    userData1,      
	    PRUint32    userData2,      
	    PRUint32    userData3,      
	    PRUint32    userData4,      
	    PRUint32    userData5,      
	    PRUint32    userData6,      
	    PRUint32    userData7       
);

/* -----------------------------------------------------------------------
** FUNCTION: PR_SetTraceOption() -- Control the Trace Facility
** 
** DESCRIPTION:
** PR_SetTraceOption() controls the Trace Facility. Depending on
** command and value, attributes of the Trace Facility may be
** changed.
** 
** INPUTS:
**  command: An enumerated value in the set of PRTraceOption.
**  value: pointer to the data to be set. Type of the data is
**  dependent on command; for each value of command, the type
**  and meaning of dereferenced value is shown.
**
**  PRTraceBufSize: unsigned long: the size of the trace buffer,
** in bytes.
** 
**  PRTraceEnable: PRTraceHandle. The trace handle to be
** enabled.
** 
**  PRTraceDisable: PRTraceHandle. The trace handle to be
** disabled.
** 
**  PRTraceSuspend: void. value must be NULL. All tracing is
** suspended.
** 
**  PRTraceResume: void. value must be NULL. Tracing for all
** previously enabled, prior to a PRTraceSuspend, is resumed.
** 
**  PRTraceStopRecording: void. value must be NULL. If recording
** (see: ** PR_RecordTraceEntries()) is being done, 
** PRTraceStopRecording causes PR_RecordTraceEntries() to return
** to its caller. If recording is not being done, this function
** has no effect.
** 
**  PRTraceSuspendRecording: void. Must be NULL. If recording is
** being done, PRTraceSuspendRecording causes further writes to
** the trace file to be suspended. Data in the in-memory
** trace buffer that would ordinarily be written to the
** trace file will not be written. Trace entries will continue
** to be entered in the in-memory buffer. If the Trace Facility
** recording is already in a suspended state, the call has no
** effect.
** 
**  PRTraceResumeRecording: void. value must be NULL. If
** recording for the Trace Facility has been previously been
** suspended, this causes recording to resume. Recording resumes
** with the next in-memory buffer segment that would be written
** if trace recording had not been suspended. If recording is
** not currently suspended, the call has no effect.
** 
**  PRTraceLockHandles: void. value must be NULL. Locks the
** trace handle lock. While the trace handle lock is held,
** calls to PR_CreateTrace() will block until the lock is
** released.
** 
**  PRTraceUnlockHandles: void. value must be NULL. Unlocks the
** trace handle lock.
** 
** OUTPUTS:
**  The operation of the Trace Facility may be changed.
** 
** RETURNS: void
** 
** RESTRICTIONS:
** 
*/
#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_SET_TRACE_OPTION(command,value)\
    PR_SetTraceOption((command),(value))
#else
#define PR_SET_TRACE_OPTION(command,value)
#endif

NSPR_API(void) 
	PR_SetTraceOption( 
	    PRTraceOption command,  
	    void *value             
);


#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_GET_TRACE_OPTION(command,value)\
    PR_GetTraceOption((command),(value))
#else
#define PR_GET_TRACE_OPTION(command,value)
#endif

NSPR_API(void) 
	PR_GetTraceOption( 
    	PRTraceOption command,  
	    void *value             
);

#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_GET_TRACE_HANDLE_FROM_NAME(handle,qName,rName)\
    (handle) = PR_GetTraceHandleFromName((qName),(rName))
#else
#define PR_GET_TRACE_HANDLE_FROM_NAME(handle,qName,rName)
#endif

NSPR_API(PRTraceHandle) 
	PR_GetTraceHandleFromName( 
    	const char *qName,      
        const char *rName       
);

#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_GET_TRACE_NAME_FROM_HANDLE(handle,qName,rName,description)\
    PR_GetTraceNameFromHandle((handle),(qName),(rName),(description))
#else
#define PR_GET_TRACE_NAME_FROM_HANDLE(handle,qName,rName,description)
#endif

NSPR_API(void) 
	PR_GetTraceNameFromHandle( 
    	PRTraceHandle handle,       
	    const char **qName,         
	    const char **rName,         
    	const char **description    
);

#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_FIND_NEXT_TRACE_QNAME(next,handle)\
    (next) = PR_FindNextTraceQname((handle))
#else
#define PR_FIND_NEXT_TRACE_QNAME(next,handle)
#endif

NSPR_API(PRTraceHandle) 
	PR_FindNextTraceQname( 
        PRTraceHandle handle
);


#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_FIND_NEXT_TRACE_RNAME(next,rhandle,qhandle)\
    (next) = PR_FindNextTraceRname((rhandle),(qhandle))
#else
#define PR_FIND_NEXT_TRACE_RNAME(next,rhandle,qhandle)
#endif

NSPR_API(PRTraceHandle) 
	PR_FindNextTraceRname( 
        PRTraceHandle rhandle,
        PRTraceHandle qhandle
);

/* -----------------------------------------------------------------------
** FUNCTION: PR_RecordTraceEntries() -- Write trace entries to external media
** 
** DESCRIPTION:
** PR_RecordTraceEntries() causes entries in the in-memory trace
** buffer to be written to external media.
**
** When PR_RecordTraceEntries() is called from an application
** thread, the function appears to block until another thread
** calls PR_SetTraceOption() with the PRTraceStopRecording
** option. This suggests that PR_RecordTraceEntries() should be
** called from a user supplied thread whose only job is to
** record trace entries. 
** 
** The environment variable NSPR_TRACE_LOG controls the operation
** of this function. When NSPR_TRACE_LOG is not defined in the
** environment, no recording of trace entries occurs. When
** NSPR_TRACE_LOG is defined, the value of its definition must be
** the filename of the file to receive the trace entry buffer.
**
** PR_RecordTraceEntries() attempts to record the in-memory
** buffer to a file, subject to the setting of the environment
** variable NSPR_TRACE_LOG. It is possible because of system
** load, the thread priority of the recording thread, number of
** active trace records being written over time, and other
** variables that some trace records can be lost. ... In other
** words: don't bet the farm on getting everything.
** 
** INPUTS: none
** 
** OUTPUTS: none
** 
** RETURNS: PR_STATUS
**    PR_SUCCESS no errors were found.
**    PR_FAILURE errors were found.
** 
** RESTRICTIONS:
** Only one thread can call PR_RecordTraceEntries() within a
** process.
** 
** On error, PR_RecordTraceEntries() may return prematurely.
** 
*/
#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_RECORD_TRACE_ENTRIES()\
	PR_RecordTraceEntries()
#else
#define PR_RECORD_TRACE_ENTRIES()
#endif
    
NSPR_API(void)
	PR_RecordTraceEntries(
        void 
);

#if defined (DEBUG) || defined (FORCE_NSPR_TRACE)
#define PR_GET_TRACE_ENTRIES(buffer,count,found)\
        PR_GetTraceEntries((buffer),(count),(found))
#else
#define PR_GET_TRACE_ENTRIES(buffer,count,found)
#endif

NSPR_API(PRIntn)
    PR_GetTraceEntries(
        PRTraceEntry    *buffer,    
        PRInt32         count,      
        PRInt32         *found      
);

PR_END_EXTERN_C

#endif 

