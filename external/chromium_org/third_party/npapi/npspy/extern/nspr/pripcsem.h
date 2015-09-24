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
 * Copyright (C) 1999-2000 Netscape Communications Corporation.  All
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


#ifndef pripcsem_h___
#define pripcsem_h___

#include "prtypes.h"
#include "prio.h"

PR_BEGIN_EXTERN_C

typedef struct PRSem PRSem;


#define PR_SEM_CREATE 0x1  
#define PR_SEM_EXCL   0x2  

NSPR_API(PRSem *) PR_OpenSemaphore(
    const char *name, PRIntn flags, PRIntn mode, PRUintn value);


NSPR_API(PRStatus) PR_WaitSemaphore(PRSem *sem);


NSPR_API(PRStatus) PR_PostSemaphore(PRSem *sem);


NSPR_API(PRStatus) PR_CloseSemaphore(PRSem *sem);


NSPR_API(PRStatus) PR_DeleteSemaphore(const char *name);

PR_END_EXTERN_C

#endif 
