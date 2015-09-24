// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_WEBSOCKET_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_WEBSOCKET_HOST_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "content/common/websocket.h"

class GURL;

namespace net {
class WebSocketChannel;
class URLRequestContext;
}  

namespace IPC {
class Message;
}  

namespace content {

class WebSocketDispatcherHost;

class CONTENT_EXPORT WebSocketHost {
 public:
  WebSocketHost(int routing_id,
                WebSocketDispatcherHost* dispatcher,
                net::URLRequestContext* url_request_context);
  virtual ~WebSocketHost();

  
  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok);

 private:
  
  

  void OnAddChannelRequest(const GURL& socket_url,
                           const std::vector<std::string>& requested_protocols,
                           const GURL& origin);

  void OnSendFrame(bool fin,
                   WebSocketMessageType type,
                   const std::vector<char>& data);

  void OnFlowControl(int64 quota);

  void OnDropChannel(bool was_clean, uint16 code, const std::string& reason);

  
  scoped_ptr<net::WebSocketChannel> channel_;

  
  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHost);
};

}  

#endif  
