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
 * replace them with the notice and other provisions requiored by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef prolock_h___
#define prolock_h___

#include "prtypes.h"

PR_BEGIN_EXTERN_C



#if defined(DEBUG) || defined(FORCE_NSPR_ORDERED_LOCKS)
typedef void * PROrderedLock;
#else
#include "prlock.h"

typedef PRLock PROrderedLock;
#endif

#if defined(DEBUG) || defined(FORCE_NSPR_ORDERED_LOCKS)
#define PR_CREATE_ORDERED_LOCK(order,name)\
    PR_CreateOrderedLock((order),(name))
#else
#define PR_CREATE_ORDERED_LOCK(order) PR_NewLock()
#endif

NSPR_API(PROrderedLock *) 
    PR_CreateOrderedLock( 
        PRInt32 order,
        const char *name
);

#if defined(DEBUG) || defined(FORCE_NSPR_ORDERED_LOCKS)
#define PR_DESTROY_ORDERED_LOCK(lock) PR_DestroyOrderedLock((lock))
#else
#define PR_DESTROY_ORDERED_LOCK(lock) PR_DestroyLock((lock))
#endif

NSPR_API(void) 
    PR_DestroyOrderedLock( 
        PROrderedLock *lock 
);

#if defined(DEBUG) || defined(FORCE_NSPR_ORDERED_LOCKS)
#define PR_LOCK_ORDERED_LOCK(lock) PR_LockOrderedLock((lock))
#else
#define PR_LOCK_ORDERED_LOCK(lock) PR_Lock((lock))
#endif

NSPR_API(void) 
    PR_LockOrderedLock( 
        PROrderedLock *lock 
);

#if defined(DEBUG) || defined(FORCE_NSPR_ORDERED_LOCKS)
#define PR_UNLOCK_ORDERED_LOCK(lock) PR_UnlockOrderedLock((lock))
#else
#define PR_UNLOCK_ORDERED_LOCK(lock) PR_Unlock((lock))
#endif

NSPR_API(PRStatus) 
    PR_UnlockOrderedLock( 
        PROrderedLock *lock 
);

PR_END_EXTERN_C

#endif 
