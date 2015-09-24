// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_TCP_SOCKET_H_
#define PPAPI_CPP_TCP_SOCKET_H_

#include "ppapi/c/ppb_tcp_socket.h"
#include "ppapi/cpp/net_address.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class InstanceHandle;

template <typename T> class CompletionCallbackWithOutput;

class TCPSocket : public Resource {
 public:
  
  
  TCPSocket();

  
  
  
  
  explicit TCPSocket(const InstanceHandle& instance);

  
  
  
  
  TCPSocket(PassRef, PP_Resource resource);

  
  
  
  TCPSocket(const TCPSocket& other);

  
  virtual ~TCPSocket();

  
  
  
  
  
  TCPSocket& operator=(const TCPSocket& other);

  
  
  
  
  static bool IsAvailable();

  
  
  
  
  
  
  
  
  
  
  int32_t Bind(const NetAddress& addr, const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Connect(const NetAddress& addr, const CompletionCallback& callback);

  
  
  
  
  NetAddress GetLocalAddress() const;

  
  
  
  
  NetAddress GetRemoteAddress() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Read(char* buffer,
               int32_t bytes_to_read,
               const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  /// been written; otherwise, an error code from <code>pp_errors.h</code>.
  int32_t Write(const char* buffer,
                int32_t bytes_to_write,
                const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Listen(int32_t backlog,
                 const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  int32_t Accept(const CompletionCallbackWithOutput<TCPSocket>& callback);

  
  
  
  
  
  
  
  
  
  void Close();

  
  
  
  
  
  
  
  
  
  
  int32_t SetOption(PP_TCPSocket_Option name,
                    const Var& value,
                    const CompletionCallback& callback);
};

}  

#endif  
