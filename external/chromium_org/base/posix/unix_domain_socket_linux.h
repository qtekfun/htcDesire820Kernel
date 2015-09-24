// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_POSIX_UNIX_DOMAIN_SOCKET_LINUX_H_
#define BASE_POSIX_UNIX_DOMAIN_SOCKET_LINUX_H_

#include <stdint.h>
#include <sys/types.h>
#include <vector>

#include "base/base_export.h"

class Pickle;

class BASE_EXPORT UnixDomainSocket {
 public:
  
  static const size_t kMaxFileDescriptors;

  
  
  static bool SendMsg(int fd,
                      const void* msg,
                      size_t length,
                      const std::vector<int>& fds);

  
  
  static ssize_t RecvMsg(int fd,
                         void* msg,
                         size_t length,
                         std::vector<int>* fds);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ssize_t SendRecvMsg(int fd,
                             uint8_t* reply,
                             unsigned reply_len,
                             int* result_fd,
                             const Pickle& request);

  
  
  static ssize_t SendRecvMsgWithFlags(int fd,
                                      uint8_t* reply,
                                      unsigned reply_len,
                                      int recvmsg_flags,
                                      int* result_fd,
                                      const Pickle& request);
 private:
  
  static ssize_t RecvMsgWithFlags(int fd,
                                  void* msg,
                                  size_t length,
                                  int flags,
                                  std::vector<int>* fds);
};

#endif  
