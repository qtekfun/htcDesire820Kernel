// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_SOCKET_NSS_SSL_UTIL_H_
#define NET_SOCKET_NSS_SSL_UTIL_H_

#include <prerror.h>

namespace net {

class BoundNetLog;

void EnsureNSSSSLInit();

void LogFailedNSSFunction(const BoundNetLog& net_log,
                          const char* function,
                          const char* param);

PRErrorCode MapErrorToNSS(int result);

int MapNSSError(PRErrorCode err);

int MapNSSHandshakeError(PRErrorCode err);

}  

#endif  
