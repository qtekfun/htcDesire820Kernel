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


#ifndef prinet_h__
#define prinet_h__

#if defined(XP_UNIX) || defined(XP_OS2) || defined(XP_BEOS)
#ifdef LINUX
#undef __STRICT_ANSI__
#define __STRICT_ANSI__
#endif
#include <sys/types.h>
#include <sys/socket.h>		
#include <netinet/in.h>         
#ifdef XP_OS2
#include <sys/ioctl.h>
#endif
#ifdef XP_UNIX
#ifdef AIX
struct ether_addr;
struct sockaddr_dl;
#endif 
#include <arpa/inet.h>
#endif 
#include <netdb.h>

#if defined(FREEBSD) || defined(BSDI) || defined(QNX)
#include <rpc/types.h> 
#endif

#if defined(OS2) && !defined(INADDR_LOOPBACK)
#define INADDR_LOOPBACK 0x7f000001
#endif

#if defined(BSDI) || defined(OSF1)
#include <machine/endian.h>
#endif

#elif defined(WIN32)


#elif defined(WIN16)

#include <winsock.h>

#elif defined(XP_MAC)

#include "macsocket.h"

#else

#error Unknown platform

#endif

#endif 
