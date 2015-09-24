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

#ifndef prinit_h___
#define prinit_h___

#include "prthread.h"
#include "prtypes.h"
#include "prwin16.h"
#include <stdio.h>

PR_BEGIN_EXTERN_C


#define PR_NAME     "NSPR"

#define PR_VERSION  "4.5 Beta"
#define PR_VMAJOR   4
#define PR_VMINOR   5
#define PR_VPATCH   0
#define PR_BETA     PR_TRUE


typedef PRBool (*PRVersionCheck)(const char*);


NSPR_API(PRBool) PR_VersionCheck(const char *importedVersion);



NSPR_API(void) PR_Init(
    PRThreadType type, PRThreadPriority priority, PRUintn maxPTDs);


typedef PRIntn (PR_CALLBACK *PRPrimordialFn)(PRIntn argc, char **argv);

NSPR_API(PRIntn) PR_Initialize(
    PRPrimordialFn prmain, PRIntn argc, char **argv, PRUintn maxPTDs);

NSPR_API(PRBool) PR_Initialized(void);

NSPR_API(PRStatus) PR_Cleanup(void);

NSPR_API(void) PR_DisableClockInterrupts(void);

NSPR_API(void) PR_EnableClockInterrupts(void);

NSPR_API(void) PR_BlockClockInterrupts(void);

NSPR_API(void) PR_UnblockClockInterrupts(void);

NSPR_API(void) PR_SetConcurrency(PRUintn numCPUs);

NSPR_API(PRStatus) PR_SetFDCacheSize(PRIntn low, PRIntn high);

NSPR_API(void) PR_ProcessExit(PRIntn status);

NSPR_API(void) PR_Abort(void);


typedef struct PRCallOnceType {
    PRIntn initialized;
    PRInt32 inProgress;
    PRStatus status;
} PRCallOnceType;

typedef PRStatus (PR_CALLBACK *PRCallOnceFN)(void);

typedef PRStatus (PR_CALLBACK *PRCallOnceWithArgFN)(void *arg);

NSPR_API(PRStatus) PR_CallOnce(
    PRCallOnceType *once,
    PRCallOnceFN    func
);

NSPR_API(PRStatus) PR_CallOnceWithArg(
    PRCallOnceType      *once,
    PRCallOnceWithArgFN  func,
    void                *arg
);


PR_END_EXTERN_C

#endif 
