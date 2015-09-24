// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_WEBSOCKET_DISPATCHER_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_WEBSOCKET_DISPATCHER_HOST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "content/common/content_export.h"
#include "content/common/websocket.h"
#include "content/public/browser/browser_message_filter.h"

namespace net {
class URLRequestContext;
}  

namespace content {

struct WebSocketHandshakeRequest;
struct WebSocketHandshakeResponse;
class WebSocketHost;

class CONTENT_EXPORT WebSocketDispatcherHost : public BrowserMessageFilter {
 public:
  typedef base::Callback<net::URLRequestContext*()> GetRequestContextCallback;

  
  
  typedef base::Callback<WebSocketHost*(int)> WebSocketHostFactory;  

  
  
  
  
  enum WebSocketHostState {
    WEBSOCKET_HOST_ALIVE,
    WEBSOCKET_HOST_DELETED
  };

  explicit WebSocketDispatcherHost(
      const GetRequestContextCallback& get_context_callback);

  
  
  WebSocketDispatcherHost(
      const GetRequestContextCallback& get_context_callback,
      const WebSocketHostFactory& websocket_host_factory);

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  
  
  
  

  
  
  WebSocketHostState SendAddChannelResponse(
      int routing_id,
      bool fail,
      const std::string& selected_protocol,
      const std::string& extensions) WARN_UNUSED_RESULT;

  
  WebSocketHostState SendFrame(int routing_id,
                               bool fin,
                               WebSocketMessageType type,
                               const std::vector<char>& data);

  
  WebSocketHostState SendFlowControl(int routing_id,
                                     int64 quota) WARN_UNUSED_RESULT;

  
  WebSocketHostState SendClosing(int routing_id) WARN_UNUSED_RESULT;

  
  WebSocketHostState SendStartOpeningHandshake(
      int routing_id,
      const WebSocketHandshakeRequest& request) WARN_UNUSED_RESULT;

  
  WebSocketHostState SendFinishOpeningHandshake(
      int routing_id,
      const WebSocketHandshakeResponse& response) WARN_UNUSED_RESULT;

  
  
  WebSocketHostState DoDropChannel(
      int routing_id,
      uint16 code,
      const std::string& reason) WARN_UNUSED_RESULT;

 private:
  typedef base::hash_map<int, WebSocketHost*> WebSocketHostTable;

  virtual ~WebSocketDispatcherHost();

  WebSocketHost* CreateWebSocketHost(int routing_id);

  
  
  WebSocketHost* GetHost(int routing_id) const;

  
  
  
  
  WebSocketHostState SendOrDrop(IPC::Message* message) WARN_UNUSED_RESULT;

  
  
  void DeleteWebSocketHost(int routing_id);

  
  
  WebSocketHostTable hosts_;

  
  
  GetRequestContextCallback get_context_callback_;

  WebSocketHostFactory websocket_host_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketDispatcherHost);
};

}  

#endif  
