// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_UNIX_DOMAIN_SOCKET_UTIL_H_
#define IPC_UNIX_DOMAIN_SOCKET_UTIL_H_

#include <sys/types.h>

#include <string>

#include "ipc/ipc_export.h"

namespace base {
class FilePath;
}  

namespace IPC {

IPC_EXPORT bool CreateServerUnixDomainSocket(const base::FilePath& socket_name,
                                             int* server_listen_fd);

IPC_EXPORT bool CreateClientUnixDomainSocket(const base::FilePath& socket_name,
                                             int* client_socket);

IPC_EXPORT bool GetPeerEuid(int fd, uid_t* peer_euid);

IPC_EXPORT bool IsPeerAuthorized(int peer_fd);

IPC_EXPORT bool ServerAcceptConnection(int server_listen_fd,
                                       int* server_socket);

static const size_t kMaxSocketNameLength = 104;

}  

#endif  
