// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_UDP_SOCKET_H_
#define NET_UDP_UDP_SOCKET_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include "net/udp/udp_socket_win.h"
#elif defined(OS_POSIX)
#include "net/udp/udp_socket_libevent.h"
#endif

namespace net {

#if defined(OS_WIN)
typedef UDPSocketWin UDPSocket;
#elif defined(OS_POSIX)
typedef UDPSocketLibevent UDPSocket;
#endif

}  

#endif  
