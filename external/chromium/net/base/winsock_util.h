// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_WINSOCK_UTIL_H_
#define NET_BASE_WINSOCK_UTIL_H_
#pragma once

#include <winsock2.h>

namespace net {

void AssertEventNotSignaled(WSAEVENT hEvent);

bool ResetEventIfSignaled(WSAEVENT hEvent);

}  

#endif  
