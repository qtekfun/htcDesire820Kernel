// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_FLIP_SERVER_CREATE_LISTENER_H__
#define NET_TOOLS_FLIP_SERVER_CREATE_LISTENER_H__

#include <iosfwd>
#include <string>

namespace net {

void FlipSetNonBlocking(int fd);

int CreateListeningSocket(const std::string& host,
                          const std::string& port,
                          bool is_numeric_host_address,
                          int backlog,
                          bool reuseaddr,
                          bool reuseport,
                          bool wait_for_iface,
                          bool disable_nagle,
                          int* listen_fd);

int CreateConnectedSocket(int* connect_fd,
                          const std::string& host,
                          const std::string& port,
                          bool is_numeric_host_address,
                          bool disable_nagle);
}  

#endif  
