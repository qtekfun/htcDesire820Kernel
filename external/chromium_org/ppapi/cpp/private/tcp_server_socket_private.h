// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_TCP_SERVER_SOCKET_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_TCP_SERVER_SOCKET_PRIVATE_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/private/ppb_tcp_server_socket_private.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class InstanceHandle;

class TCPServerSocketPrivate : public Resource {
 public:
  explicit TCPServerSocketPrivate(const InstanceHandle& instance);

  
  static bool IsAvailable();

  int32_t Listen(const PP_NetAddress_Private* addr,
                 int32_t backlog,
                 const CompletionCallback& callback);
  
  
  
  
  int32_t Accept(PP_Resource* socket,
                 const CompletionCallback& callback);
  int32_t GetLocalAddress(PP_NetAddress_Private* addr);
  void StopListening();
};

}  

#endif  
