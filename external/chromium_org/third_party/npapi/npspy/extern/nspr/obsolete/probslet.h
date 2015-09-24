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


#if defined(PROBSLET_H)
#else
#define PROBSLET_H

#include "prio.h"

PR_BEGIN_EXTERN_C

NSPR_API(PRStatus) PR_Yield(void);


#ifndef PR_MAX_SELECT_DESC
#define PR_MAX_SELECT_DESC 1024
#endif
typedef struct PR_fd_set {
    PRUint32      hsize;
    PRFileDesc   *harray[PR_MAX_SELECT_DESC];
    PRUint32      nsize;
    PRInt32       narray[PR_MAX_SELECT_DESC];
} PR_fd_set;

NSPR_API(PRInt32) PR_Select(
    PRInt32 num, PR_fd_set *readfds, PR_fd_set *writefds,
    PR_fd_set *exceptfds, PRIntervalTime timeout);


NSPR_API(void)        PR_FD_ZERO(PR_fd_set *set);
NSPR_API(void)        PR_FD_SET(PRFileDesc *fd, PR_fd_set *set);
NSPR_API(void)        PR_FD_CLR(PRFileDesc *fd, PR_fd_set *set);
NSPR_API(PRInt32)     PR_FD_ISSET(PRFileDesc *fd, PR_fd_set *set);
NSPR_API(void)        PR_FD_NSET(PRInt32 osfd, PR_fd_set *set);
NSPR_API(void)        PR_FD_NCLR(PRInt32 osfd, PR_fd_set *set);
NSPR_API(PRInt32)     PR_FD_NISSET(PRInt32 osfd, PR_fd_set *set);

#ifndef NO_NSPR_10_SUPPORT
#ifdef XP_MAC
#include <stat.h>
#else
#include <sys/stat.h>
#endif

NSPR_API(PRInt32) PR_Stat(const char *path, struct stat *buf);
#endif 

PR_END_EXTERN_C

#endif 

