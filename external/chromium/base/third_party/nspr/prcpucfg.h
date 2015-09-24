// Copyright 2008, Google Inc.
//    * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef BASE_THIRD_PARTY_NSPR_PRCPUCFG_H__
#define BASE_THIRD_PARTY_NSPR_PRCPUCFG_H__

#if defined(WIN32)
#include "base/third_party/nspr/prcpucfg_win.h"
#elif defined(__APPLE__)
#include "base/third_party/nspr/prcpucfg_mac.h"
#elif defined(__native_client__)
#include "base/third_party/nspr/prcpucfg_nacl.h"
#elif defined(__linux__) || defined(ANDROID)
#include "base/third_party/nspr/prcpucfg_linux.h"
#elif defined(__FreeBSD__)
#include "base/third_party/nspr/prcpucfg_freebsd.h"
#elif defined(__OpenBSD__)
#include "base/third_party/nspr/prcpucfg_openbsd.h"
#elif defined(__sun)
#include "base/third_party/nspr/prcpucfg_solaris.h"
#else
#error Provide a prcpucfg.h appropriate for your platform
#endif

#endif  
