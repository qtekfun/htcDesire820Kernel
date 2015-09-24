// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_WEBSOCKET_H_
#define PPAPI_CPP_WEBSOCKET_H_

#include "ppapi/c/ppb_websocket.h"
#include "ppapi/cpp/resource.h"


#ifdef SendMessage
#undef SendMessage
#endif

namespace pp {

class CompletionCallback;
class InstanceHandle;
class Var;

class WebSocket : public Resource {
 public:
  
  
  
  
  explicit WebSocket(const InstanceHandle& instance);

  
  virtual ~WebSocket();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Connect(const Var& url, const Var protocols[],
                  uint32_t protocol_count, const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Close(uint16_t code, const Var& reason,
                const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t ReceiveMessage(Var* message,
                         const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t SendMessage(const Var& message);

  
  
  
  
  
  uint64_t GetBufferedAmount();

  
  
  
  
  uint16_t GetCloseCode();

  
  
  
  
  
  
  Var GetCloseReason();

  
  
  
  
  
  
  
  bool GetCloseWasClean();

  
  
  
  
  
  
  
  
  Var GetExtensions();

  
  
  
  
  
  
  
  Var GetProtocol();

  
  
  
  
  
  
  PP_WebSocketReadyState GetReadyState();

  
  
  
  
  
  
  Var GetURL();
};

}  

#endif  
