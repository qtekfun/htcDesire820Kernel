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

#ifndef prthread_h___
#define prthread_h___

#include "prtypes.h"
#include "prinrval.h"

PR_BEGIN_EXTERN_C

typedef struct PRThread PRThread;
typedef struct PRThreadStack PRThreadStack;

typedef enum PRThreadType {
    PR_USER_THREAD,
    PR_SYSTEM_THREAD
} PRThreadType;

typedef enum PRThreadScope {
    PR_LOCAL_THREAD,
    PR_GLOBAL_THREAD,
    PR_GLOBAL_BOUND_THREAD
} PRThreadScope;

typedef enum PRThreadState {
    PR_JOINABLE_THREAD,
    PR_UNJOINABLE_THREAD
} PRThreadState;

typedef enum PRThreadPriority
{
    PR_PRIORITY_FIRST = 0,      
    PR_PRIORITY_LOW = 0,        
    PR_PRIORITY_NORMAL = 1,     
    PR_PRIORITY_HIGH = 2,       
    PR_PRIORITY_URGENT = 3,     
    PR_PRIORITY_LAST = 3        
} PRThreadPriority;

NSPR_API(PRThread*) PR_CreateThread(PRThreadType type,
                     void (PR_CALLBACK *start)(void *arg),
                     void *arg,
                     PRThreadPriority priority,
                     PRThreadScope scope,
                     PRThreadState state,
                     PRUint32 stackSize);

NSPR_API(PRStatus) PR_JoinThread(PRThread *thread);

NSPR_API(PRThread*) PR_GetCurrentThread(void);
#ifndef NO_NSPR_10_SUPPORT
#define PR_CurrentThread() PR_GetCurrentThread() 
#endif 

NSPR_API(PRThreadPriority) PR_GetThreadPriority(const PRThread *thread);

NSPR_API(void) PR_SetThreadPriority(PRThread *thread, PRThreadPriority priority);

typedef void (PR_CALLBACK *PRThreadPrivateDTOR)(void *priv);

NSPR_API(PRStatus) PR_NewThreadPrivateIndex(
    PRUintn *newIndex, PRThreadPrivateDTOR destructor);

NSPR_API(PRStatus) PR_SetThreadPrivate(PRUintn tpdIndex, void *priv);

NSPR_API(void*) PR_GetThreadPrivate(PRUintn tpdIndex);

NSPR_API(PRStatus) PR_Interrupt(PRThread *thread);

NSPR_API(void) PR_ClearInterrupt(void);

NSPR_API(void) PR_BlockInterrupt(void);

NSPR_API(void) PR_UnblockInterrupt(void);

NSPR_API(PRStatus) PR_Sleep(PRIntervalTime ticks);

NSPR_API(PRThreadScope) PR_GetThreadScope(const PRThread *thread);

NSPR_API(PRThreadType) PR_GetThreadType(const PRThread *thread);

NSPR_API(PRThreadState) PR_GetThreadState(const PRThread *thread);

PR_END_EXTERN_C

#endif 
