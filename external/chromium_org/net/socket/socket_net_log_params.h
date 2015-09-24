// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKET_NET_LOG_PARAMS_H_
#define NET_SOCKET_SOCKET_NET_LOG_PARAMS_H_

#include "net/base/net_log.h"
#include "net/base/sys_addrinfo.h"

namespace net {

class HostPortPair;
class IPEndPoint;

NetLog::ParametersCallback CreateNetLogSocketErrorCallback(int net_error,
                                                           int os_error);

NetLog::ParametersCallback CreateNetLogHostPortPairCallback(
    const HostPortPair* host_and_port);

NetLog::ParametersCallback CreateNetLogIPEndPointCallback(
    const IPEndPoint* address);

NetLog::ParametersCallback CreateNetLogSourceAddressCallback(
    const struct sockaddr* net_address,
    socklen_t address_len);

}  

#endif  
