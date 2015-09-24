// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DNS_RELOADER_H_
#define NET_BASE_DNS_RELOADER_H_

#include "build/build_config.h"

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_OPENBSD)
namespace net {

void EnsureDnsReloaderInit();

void DnsReloaderMaybeReload();

}  
#endif  

#endif  
