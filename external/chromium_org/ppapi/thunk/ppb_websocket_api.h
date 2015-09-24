// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_THUNK_WEBSOCKET_API_H_
#define PPAPI_THUNK_WEBSOCKET_API_H_

#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/ppb_websocket.h"
#include "ppapi/thunk/ppapi_thunk_export.h"

namespace ppapi {

class TrackedCallback;

namespace thunk {

class PPAPI_THUNK_EXPORT PPB_WebSocket_API {
 public:
  virtual ~PPB_WebSocket_API() {}

  
  
  
  virtual int32_t Connect(const PP_Var& url,
                          const PP_Var protocols[],
                          uint32_t protocol_count,
                          scoped_refptr<TrackedCallback> callback) = 0;

  
  
  virtual int32_t Close(uint16_t code,
                        const PP_Var& reason,
                        scoped_refptr<TrackedCallback> callback) = 0;

  
  
  
  virtual int32_t ReceiveMessage(PP_Var* message,
                                 scoped_refptr<TrackedCallback> callback) = 0;

  
  
  virtual int32_t SendMessage(const PP_Var& message) = 0;

  
  virtual uint64_t GetBufferedAmount() = 0;

  
  
  
  virtual uint16_t GetCloseCode() = 0;

  
  
  
  virtual PP_Var GetCloseReason() = 0;

  
  
  virtual PP_Bool GetCloseWasClean() = 0;

  
  virtual PP_Var GetExtensions() = 0;

  
  virtual PP_Var GetProtocol() = 0;

  
  virtual PP_WebSocketReadyState GetReadyState() = 0;

  
  virtual PP_Var GetURL() = 0;
};

}  
}  

#endif  
