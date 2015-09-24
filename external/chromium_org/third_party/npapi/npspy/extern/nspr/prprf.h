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

#ifndef prprf_h___
#define prprf_h___

#include "prtypes.h"
#include "prio.h"
#include <stdio.h>
#include <stdarg.h>

PR_BEGIN_EXTERN_C

/*
** sprintf into a fixed size buffer. Guarantees that a NUL is at the end
** of the buffer. Returns the length of the written output, NOT including
** the NUL, or (PRUint32)-1 if an error occurs.
*/
NSPR_API(PRUint32) PR_snprintf(char *out, PRUint32 outlen, const char *fmt, ...);

NSPR_API(char*) PR_smprintf(const char *fmt, ...);

NSPR_API(void) PR_smprintf_free(char *mem);

NSPR_API(char*) PR_sprintf_append(char *last, const char *fmt, ...);

typedef PRIntn (*PRStuffFunc)(void *arg, const char *s, PRUint32 slen);

NSPR_API(PRUint32) PR_sxprintf(PRStuffFunc f, void *arg, const char *fmt, ...);

NSPR_API(PRUint32) PR_fprintf(struct PRFileDesc* fd, const char *fmt, ...);

NSPR_API(PRUint32) PR_vsnprintf(char *out, PRUint32 outlen, const char *fmt, va_list ap);
NSPR_API(char*) PR_vsmprintf(const char *fmt, va_list ap);
NSPR_API(char*) PR_vsprintf_append(char *last, const char *fmt, va_list ap);
NSPR_API(PRUint32) PR_vsxprintf(PRStuffFunc f, void *arg, const char *fmt, va_list ap);
NSPR_API(PRUint32) PR_vfprintf(struct PRFileDesc* fd, const char *fmt, va_list ap);


NSPR_API(PRInt32) PR_sscanf(const char *buf, const char *fmt, ...);

PR_END_EXTERN_C

#endif 
