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

#ifndef prdtoa_h___
#define prdtoa_h___

#include "prtypes.h"

PR_BEGIN_EXTERN_C

#if defined(HAVE_WATCOM_BUG_1)
PRFloat64 __pascal __loadds __export
#else
NSPR_API(PRFloat64)
#endif
PR_strtod(const char *s00, char **se);

NSPR_API(void) PR_cnvtf(char *buf, PRIntn bufsz, PRIntn prcsn, PRFloat64 fval);

NSPR_API(PRStatus) PR_dtoa(PRFloat64 d, PRIntn mode, PRIntn ndigits,
	PRIntn *decpt, PRIntn *sign, char **rve, char *buf, PRSize bufsize);

PR_END_EXTERN_C

#endif 
