// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_UDP_SOCKET_H_
#define PPAPI_CPP_UDP_SOCKET_H_

#include "ppapi/c/ppb_udp_socket.h"
#include "ppapi/cpp/net_address.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class InstanceHandle;
class Var;

template <typename T> class CompletionCallbackWithOutput;

class UDPSocket : public Resource {
 public:
  
  
  UDPSocket();

  
  
  
  
  explicit UDPSocket(const InstanceHandle& instance);

  
  
  
  
  UDPSocket(PassRef, PP_Resource resource);

  
  
  
  UDPSocket(const UDPSocket& other);

  
  virtual ~UDPSocket();

  
  
  
  
  
  UDPSocket& operator=(const UDPSocket& other);

  
  
  
  
  static bool IsAvailable();

  
  
  
  
  
  
  
  
  
  
  int32_t Bind(const NetAddress& addr,
               const CompletionCallback& callback);

  
  
  
  
  NetAddress GetBoundAddress();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t RecvFrom(
      char* buffer,
      int32_t num_bytes,
      const CompletionCallbackWithOutput<NetAddress>& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t SendTo(const char* buffer,
                 int32_t num_bytes,
                 const NetAddress& addr,
                 const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  void Close();

  
  
  
  
  
  
  
  
  
  
  int32_t SetOption(PP_UDPSocket_Option name,
                    const Var& value,
                    const CompletionCallback& callback);
};

}  

#endif  
