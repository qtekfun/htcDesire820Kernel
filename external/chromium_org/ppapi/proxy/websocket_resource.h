// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_WEBSOCKET_RESOURCE_H_
#define PPAPI_PROXY_WEBSOCKET_RESOURCE_H_

#include <queue>

#include "ppapi/c/ppb_websocket.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/shared_impl/tracked_callback.h"
#include "ppapi/thunk/ppb_websocket_api.h"

namespace ppapi {

class StringVar;
class Var;

namespace proxy {

class PPAPI_PROXY_EXPORT WebSocketResource
    : public PluginResource,
      public NON_EXPORTED_BASE(thunk::PPB_WebSocket_API) {
 public:
  WebSocketResource(Connection connection, PP_Instance instance);
  virtual ~WebSocketResource();

  
  virtual thunk::PPB_WebSocket_API* AsPPB_WebSocket_API() OVERRIDE;

  
  virtual int32_t Connect(const PP_Var& url,
                          const PP_Var protocols[],
                          uint32_t protocol_count,
                          scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t Close(uint16_t code,
                        const PP_Var& reason,
                        scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t ReceiveMessage(
      PP_Var* message,
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t SendMessage(const PP_Var& message) OVERRIDE;
  virtual uint64_t GetBufferedAmount() OVERRIDE;
  virtual uint16_t GetCloseCode() OVERRIDE;
  virtual PP_Var GetCloseReason() OVERRIDE;
  virtual PP_Bool GetCloseWasClean() OVERRIDE;
  virtual PP_Var GetExtensions() OVERRIDE;
  virtual PP_Var GetProtocol() OVERRIDE;
  virtual PP_WebSocketReadyState GetReadyState() OVERRIDE;
  virtual PP_Var GetURL() OVERRIDE;

 private:
  
  virtual void OnReplyReceived(const ResourceMessageReplyParams& params,
                               const IPC::Message& msg) OVERRIDE;

  
  void OnPluginMsgConnectReply(const ResourceMessageReplyParams& params,
                               const std::string& url,
                               const std::string& protocol);
  void OnPluginMsgCloseReply(const ResourceMessageReplyParams& params,
                             unsigned long buffered_amount,
                             bool was_clean,
                             unsigned short code,
                             const std::string& reason);
  void OnPluginMsgReceiveTextReply(const ResourceMessageReplyParams& params,
                                   const std::string& message);
  void OnPluginMsgReceiveBinaryReply(const ResourceMessageReplyParams& params,
                                     const std::vector<uint8_t>& message);
  void OnPluginMsgErrorReply(const ResourceMessageReplyParams& params);
  void OnPluginMsgBufferedAmountReply(const ResourceMessageReplyParams& params,
                                      unsigned long buffered_amount);
  void OnPluginMsgStateReply(const ResourceMessageReplyParams& params,
                             int32_t state);
  void OnPluginMsgClosedReply(const ResourceMessageReplyParams& params,
                              unsigned long buffered_amount,
                              bool was_clean,
                              unsigned short code,
                              const std::string& reason);

  
  
  
  
  int32_t DoReceive();

  
  scoped_refptr<TrackedCallback> connect_callback_;
  scoped_refptr<TrackedCallback> close_callback_;
  scoped_refptr<TrackedCallback> receive_callback_;

  
  
  PP_WebSocketReadyState state_;

  
  
  
  bool error_was_received_;

  
  
  PP_Var* receive_callback_var_;

  
  std::queue<scoped_refptr<Var> > received_messages_;

  
  scoped_refptr<StringVar> empty_string_;

  
  
  uint16_t close_code_;

  
  
  scoped_refptr<StringVar> close_reason_;

  
  
  PP_Bool close_was_clean_;

  
  
  scoped_refptr<StringVar> extensions_;

  
  
  scoped_refptr<StringVar> protocol_;

  
  
  scoped_refptr<StringVar> url_;

  
  
  
  
  
  uint64_t buffered_amount_;

  
  
  
  
  uint64_t buffered_amount_after_close_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketResource);
};

}  
}  

#endif  
