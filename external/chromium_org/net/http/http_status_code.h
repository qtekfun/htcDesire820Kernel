// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STATUS_CODE_H_
#define NET_HTTP_HTTP_STATUS_CODE_H_

#include "net/base/net_export.h"

namespace net {

enum HttpStatusCode {

#define HTTP_STATUS(label, code, reason) HTTP_ ## label = code,
#include "net/http/http_status_code_list.h"
#undef HTTP_STATUS

};

NET_EXPORT const char* GetHttpReasonPhrase(HttpStatusCode code);

}  

#endif  
