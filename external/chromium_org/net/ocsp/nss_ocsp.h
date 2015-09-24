// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_OCSP_NSS_OCSP_H_
#define NET_OCSP_NSS_OCSP_H_

#include "net/base/net_export.h"

namespace net {

class URLRequestContext;

NET_EXPORT void SetMessageLoopForNSSHttpIO();

NET_EXPORT void EnsureNSSHttpIOInit();

NET_EXPORT void ShutdownNSSHttpIO();

NET_EXPORT void ResetNSSHttpIOForTesting();

NET_EXPORT void SetURLRequestContextForNSSHttpIO(
    URLRequestContext* request_context);

}  

#endif  
