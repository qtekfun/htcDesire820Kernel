// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_NETLOG_PARAMS_H_
#define NET_URL_REQUEST_URL_REQUEST_NETLOG_PARAMS_H_

#include <string>

#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"

class GURL;

namespace base {
class Value;
}

namespace net {

NET_EXPORT base::Value* NetLogURLRequestStartCallback(
    const GURL* url,
    const std::string* method,
    int load_flags,
    RequestPriority priority,
    int64 upload_id,
    NetLog::LogLevel );

NET_EXPORT bool StartEventLoadFlagsFromEventParams(
    const base::Value* event_params,
    int* load_flags);

}  

#endif  
