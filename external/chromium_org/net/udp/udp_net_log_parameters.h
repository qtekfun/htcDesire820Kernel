// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_UDP_NET_LOG_PARAMETERS_H_
#define NET_UDP_UDP_NET_LOG_PARAMETERS_H_

#include "net/base/net_log.h"

namespace net {

class IPEndPoint;

NetLog::ParametersCallback CreateNetLogUDPDataTranferCallback(
    int byte_count,
    const char* bytes,
    const IPEndPoint* address);

NetLog::ParametersCallback CreateNetLogUDPConnectCallback(
    const IPEndPoint* address);

}  

#endif  
