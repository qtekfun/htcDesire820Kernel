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

#ifndef prmem_h___
#define prmem_h___

#include "prtypes.h"
#include <stddef.h>
#include <stdlib.h>

PR_BEGIN_EXTERN_C



NSPR_API(void *) PR_Malloc(PRUint32 size);

NSPR_API(void *) PR_Calloc(PRUint32 nelem, PRUint32 elsize);

NSPR_API(void *) PR_Realloc(void *ptr, PRUint32 size);

NSPR_API(void) PR_Free(void *ptr);


#define PR_MALLOC(_bytes) (PR_Malloc((_bytes)))

#define PR_NEW(_struct) ((_struct *) PR_MALLOC(sizeof(_struct)))

#define PR_REALLOC(_ptr, _size) (PR_Realloc((_ptr), (_size)))

#define PR_CALLOC(_size) (PR_Calloc(1, (_size)))

#define PR_NEWZAP(_struct) ((_struct*)PR_Calloc(1, sizeof(_struct)))

#define PR_DELETE(_ptr) { PR_Free(_ptr); (_ptr) = NULL; }

#define PR_FREEIF(_ptr)	if (_ptr) PR_DELETE(_ptr)

PR_END_EXTERN_C

#endif 
