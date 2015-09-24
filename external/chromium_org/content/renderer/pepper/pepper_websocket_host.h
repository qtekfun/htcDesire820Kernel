// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_WEBSOCKET_HOST_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_WEBSOCKET_HOST_H_

#include <queue>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/resource_host.h"
#include "ppapi/proxy/resource_message_params.h"
#include "third_party/WebKit/public/web/WebSocket.h"
#include "third_party/WebKit/public/web/WebSocketClient.h"

namespace ppapi {
class StringVar;
class Var;
}  

namespace content {

class RendererPpapiHost;

class CONTENT_EXPORT PepperWebSocketHost
    : public ppapi::host::ResourceHost,
      public NON_EXPORTED_BASE(::blink::WebSocketClient) {
 public:
  explicit PepperWebSocketHost(RendererPpapiHost* host,
                               PP_Instance instance,
                               PP_Resource resource);
  virtual ~PepperWebSocketHost();

  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;

  
  virtual void didConnect();
  virtual void didReceiveMessage(const blink::WebString& message);
  virtual void didReceiveArrayBuffer(const blink::WebArrayBuffer& binaryData);
  virtual void didReceiveMessageError();
  virtual void didUpdateBufferedAmount(unsigned long buffered_amount);
  virtual void didStartClosingHandshake();
  virtual void didClose(unsigned long unhandled_buffered_amount,
                        ClosingHandshakeCompletionStatus status,
                        unsigned short code,
                        const blink::WebString& reason);
 private:
  
  int32_t OnHostMsgConnect(ppapi::host::HostMessageContext* context,
                           const std::string& url,
                           const std::vector<std::string>& protocols);
  int32_t OnHostMsgClose(ppapi::host::HostMessageContext* context,
                         int32_t code,
                         const std::string& reason);
  int32_t OnHostMsgSendText(ppapi::host::HostMessageContext* context,
                            const std::string& message);
  int32_t OnHostMsgSendBinary(ppapi::host::HostMessageContext* context,
                              const std::vector<uint8_t>& message);
  int32_t OnHostMsgFail(ppapi::host::HostMessageContext* context,
                        const std::string& message);

  
  RendererPpapiHost* renderer_ppapi_host_;

  
  ppapi::host::ReplyMessageContext connect_reply_;
  ppapi::host::ReplyMessageContext close_reply_;

  
  std::string url_;

  
  bool connecting_;

  
  bool initiating_close_;

  
  bool accepting_close_;

  
  
  bool error_was_received_;

  
  
  scoped_ptr<blink::WebSocket> websocket_;

  DISALLOW_COPY_AND_ASSIGN(PepperWebSocketHost);
};

}  

#endif  
