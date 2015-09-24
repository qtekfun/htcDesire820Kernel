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

#if defined(_PRMWAIT_H)
#else
#define _PRMWAIT_H

#include "prio.h"
#include "prtypes.h"
#include "prclist.h"

PR_BEGIN_EXTERN_C


typedef struct PRWaitGroup PRWaitGroup;

typedef enum PRMWStatus
{
    PR_MW_PENDING = 1,
    PR_MW_SUCCESS = 0,
    PR_MW_FAILURE = -1,
    PR_MW_TIMEOUT = -2,
    PR_MW_INTERRUPT = -3
} PRMWStatus;

typedef struct PRMemoryDescriptor
{
    void *start;                
    PRSize length;              
} PRMemoryDescriptor;

typedef struct PRMWaitClientData PRMWaitClientData;

typedef struct PRRecvWait 
{
    PRCList internal;           

    PRFileDesc *fd;             
    PRMWStatus outcome;         
    PRIntervalTime timeout;     

    PRInt32 bytesRecv;          
    PRMemoryDescriptor buffer;  
    PRMWaitClientData *client;  
} PRRecvWait;

typedef struct PRMWaitEnumerator PRMWaitEnumerator;


NSPR_API(PRStatus) PR_AddWaitFileDesc(PRWaitGroup *group, PRRecvWait *desc);

NSPR_API(PRRecvWait*) PR_WaitRecvReady(PRWaitGroup *group);

NSPR_API(PRStatus) PR_CancelWaitFileDesc(PRWaitGroup *group, PRRecvWait *desc);

NSPR_API(PRRecvWait*) PR_CancelWaitGroup(PRWaitGroup *group);

NSPR_API(PRWaitGroup*) PR_CreateWaitGroup(PRInt32 size);

NSPR_API(PRStatus) PR_DestroyWaitGroup(PRWaitGroup *group);

NSPR_API(PRMWaitEnumerator*) PR_CreateMWaitEnumerator(PRWaitGroup *group);

NSPR_API(PRStatus) PR_DestroyMWaitEnumerator(PRMWaitEnumerator* enumerator);

NSPR_API(PRRecvWait*) PR_EnumerateWaitGroup(
    PRMWaitEnumerator *enumerator, const PRRecvWait *previous);
   
PR_END_EXTERN_C

#endif 

