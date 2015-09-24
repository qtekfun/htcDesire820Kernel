// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_SOCKET_UTILS_H_
#define NET_TOOLS_QUIC_QUIC_SOCKET_UTILS_H_

#include <stddef.h>
#include <sys/socket.h>
#include <string>

#include "net/base/ip_endpoint.h"
#include "net/quic/quic_protocol.h"

namespace net {
namespace tools {

class QuicSocketUtils {
 public:
  
  
  
  static IPAddressNumber GetAddressFromMsghdr(struct msghdr *hdr);

  
  
  static bool GetOverflowFromMsghdr(struct msghdr *hdr, int *dropped_packets);

  
  
  static int SetGetAddressInfo(int fd, int address_family);

  
  
  
  
  
  
  
  
  
  static int ReadPacket(int fd, char* buffer, size_t buf_len,
                        int* dropped_packets,
                        IPAddressNumber* self_address,
                        IPEndPoint* peer_address);

  
  // status to WRITE_STATUS_OK and sets bytes_written.  Otherwise sets the
  
  
  static WriteResult WritePacket(int fd, const char* buffer, size_t buf_len,
                                 const IPAddressNumber& self_address,
                                 const IPEndPoint& peer_address);
};

}  
}  

#endif  
