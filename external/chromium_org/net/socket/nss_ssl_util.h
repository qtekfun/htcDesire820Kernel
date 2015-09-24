// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_SOCKET_NSS_SSL_UTIL_H_
#define NET_SOCKET_NSS_SSL_UTIL_H_

#include <prerror.h>
#include <prio.h>

#include "net/base/net_export.h"

namespace net {

class BoundNetLog;

NET_EXPORT void EnsureNSSSSLInit();

void LogFailedNSSFunction(const BoundNetLog& net_log,
                          const char* function,
                          const char* param);

PRErrorCode MapErrorToNSS(int result);

PRFileDesc* GetNSSModelSocket();

int MapNSSError(PRErrorCode err);

}  

#endif  
