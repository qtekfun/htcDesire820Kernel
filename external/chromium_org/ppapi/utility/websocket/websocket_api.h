// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_WEBSOCKET_WEBSOCKET_API_H_
#define PPAPI_UTILITY_WEBSOCKET_WEBSOCKET_API_H_

#include "ppapi/c/ppb_websocket.h"


namespace pp {

class CompletionCallback;
class Instance;
class Var;

class WebSocketAPI {
 public:
  
  explicit WebSocketAPI(Instance* instance);

  
  virtual ~WebSocketAPI();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Connect(const Var& url, const Var protocols[],
                  uint32_t protocol_count);

  
  
  
  
  
  
  
  
  
  
  int32_t Close(uint16_t code, const Var& reason);

  
  
  
  
  
  
  
  
  
  int32_t Send(const Var& data);

  
  
  
  
  
  uint64_t GetBufferedAmount();

  
  
  
  
  
  
  Var GetExtensions();

  
  
  
  
  
  Var GetProtocol();

  
  
  
  
  
  PP_WebSocketReadyState GetReadyState();

  
  
  
  
  Var GetURL();

  
  
  virtual void WebSocketDidOpen() = 0;

  
  
  virtual void WebSocketDidClose(bool wasClean,
                                 uint16_t code,
                                 const Var& reason) = 0;

  
  virtual void HandleWebSocketMessage(const Var& message) = 0;

  
  
  
  virtual void HandleWebSocketError() = 0;

 private:
  class Implement;
  Implement* impl_;
};

}  

#endif  
