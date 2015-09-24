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


#ifndef pprio_h___
#define pprio_h___

#include "prtypes.h"
#include "prio.h"

PR_BEGIN_EXTERN_C


NSPR_API(const PRIOMethods*)    PR_GetFileMethods(void);
NSPR_API(const PRIOMethods*)    PR_GetTCPMethods(void);
NSPR_API(const PRIOMethods*)    PR_GetUDPMethods(void);
NSPR_API(const PRIOMethods*)    PR_GetPipeMethods(void);

NSPR_API(PRInt32)      PR_FileDesc2NativeHandle(PRFileDesc *);
NSPR_API(void)         PR_ChangeFileDescNativeHandle(PRFileDesc *, PRInt32);
NSPR_API(PRFileDesc*)  PR_AllocFileDesc(PRInt32 osfd,
                                         const PRIOMethods *methods);
NSPR_API(void)         PR_FreeFileDesc(PRFileDesc *fd);
NSPR_API(PRFileDesc*)  PR_ImportFile(PRInt32 osfd);
NSPR_API(PRFileDesc*)  PR_ImportPipe(PRInt32 osfd);
NSPR_API(PRFileDesc*)  PR_ImportTCPSocket(PRInt32 osfd);
NSPR_API(PRFileDesc*)  PR_ImportUDPSocket(PRInt32 osfd);



NSPR_API(PRFileDesc*)	PR_CreateSocketPollFd(PRInt32 osfd);


NSPR_API(PRStatus) PR_DestroySocketPollFd(PRFileDesc *fd);



#ifdef WIN32

#define PR_SOCK_STREAM 1
#define PR_SOCK_DGRAM 2

#else 

#define PR_SOCK_STREAM SOCK_STREAM
#define PR_SOCK_DGRAM SOCK_DGRAM

#endif 

NSPR_API(PRFileDesc*)	PR_Socket(PRInt32 domain, PRInt32 type, PRInt32 proto);

NSPR_API(PRStatus) PR_LockFile(PRFileDesc *fd);

NSPR_API(PRStatus) PR_TLockFile(PRFileDesc *fd);

NSPR_API(PRStatus) PR_UnlockFile(PRFileDesc *fd);

NSPR_API(PRInt32) PR_EmulateAcceptRead(PRFileDesc *sd, PRFileDesc **nd,
    PRNetAddr **raddr, void *buf, PRInt32 amount, PRIntervalTime timeout);

NSPR_API(PRInt32) PR_EmulateSendFile(
    PRFileDesc *networkSocket, PRSendFileData *sendData,
    PRTransmitFileFlags flags, PRIntervalTime timeout);

#ifdef WIN32
NSPR_API(PRInt32) PR_NTFast_AcceptRead(PRFileDesc *sd, PRFileDesc **nd,
              PRNetAddr **raddr, void *buf, PRInt32 amount, PRIntervalTime t);

typedef void (*_PR_AcceptTimeoutCallback)(void *);

NSPR_API(PRInt32) PR_NTFast_AcceptRead_WithTimeoutCallback(
              PRFileDesc *sd, 
              PRFileDesc **nd,
              PRNetAddr **raddr, 
              void *buf, 
              PRInt32 amount, 
              PRIntervalTime t,
              _PR_AcceptTimeoutCallback callback, 
              void *callback_arg);

NSPR_API(PRFileDesc*)	PR_NTFast_Accept(PRFileDesc *fd, PRNetAddr *addr,
                                                PRIntervalTime timeout);

NSPR_API(void) PR_NTFast_UpdateAcceptContext(PRFileDesc *acceptSock, 
                                        PRFileDesc *listenSock);


NSPR_API(PRStatus) PR_NT_CancelIo(PRFileDesc *fd);


#endif 

#ifdef XP_MAC
NSPR_API(void) PR_Init_Log(void);
#endif


PR_END_EXTERN_C

#endif 
