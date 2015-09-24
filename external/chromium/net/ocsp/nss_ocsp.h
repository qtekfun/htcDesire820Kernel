// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_OCSP_NSS_OCSP_H_
#define NET_OCSP_NSS_OCSP_H_
#pragma once

namespace net {

class URLRequestContext;

void SetMessageLoopForOCSP();

void EnsureOCSPInit();

void ShutdownOCSP();

void SetURLRequestContextForOCSP(URLRequestContext* request_context);
URLRequestContext* GetURLRequestContextForOCSP();

}  

#endif  
