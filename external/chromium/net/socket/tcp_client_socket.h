// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_CLIENT_SOCKET_H_
#define NET_SOCKET_TCP_CLIENT_SOCKET_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include "net/socket/tcp_client_socket_win.h"
#elif defined(OS_POSIX)
#include "net/socket/tcp_client_socket_libevent.h"
#endif

namespace net {

#if defined(OS_WIN)
typedef TCPClientSocketWin TCPClientSocket;
#elif defined(OS_POSIX)
typedef TCPClientSocketLibevent TCPClientSocket;
#endif

void set_tcp_fastopen_enabled(bool value);

bool is_tcp_fastopen_enabled();

}  

#endif  
