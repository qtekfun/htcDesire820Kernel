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


#if !defined(PRPDCE_H)
#define PRPDCE_H

#include "prlock.h"
#include "prcvar.h"
#include "prtypes.h"
#include "prinrval.h"

PR_BEGIN_EXTERN_C

#define _PR_NAKED_CV_LOCK (PRLock*)0xdce1dce1

NSPR_API(PRStatus) PRP_TryLock(PRLock *lock);

NSPR_API(PRCondVar*) PRP_NewNakedCondVar(void);

NSPR_API(void) PRP_DestroyNakedCondVar(PRCondVar *cvar);

NSPR_API(PRStatus) PRP_NakedWait(
	PRCondVar *cvar, PRLock *lock, PRIntervalTime timeout);

NSPR_API(PRStatus) PRP_NakedNotify(PRCondVar *cvar);

NSPR_API(PRStatus) PRP_NakedBroadcast(PRCondVar *cvar);

PR_END_EXTERN_C

#endif 
