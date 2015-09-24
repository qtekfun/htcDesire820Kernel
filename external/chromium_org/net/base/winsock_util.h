// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_WINSOCK_UTIL_H_
#define NET_BASE_WINSOCK_UTIL_H_

#include <winsock2.h>

#include "net/base/net_export.h"

namespace net {

void AssertEventNotSignaled(WSAEVENT hEvent);

bool ResetEventIfSignaled(WSAEVENT hEvent);

}  

#endif  
