// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_AGENT_FILTER_H_
#define CONTENT_RENDERER_DEVTOOLS_DEVTOOLS_AGENT_FILTER_H_

#include <string>

#include "ipc/ipc_channel_proxy.h"

struct DevToolsMessageData;

namespace base {
class MessageLoop;
}

namespace content {

class DevToolsAgentFilter : public IPC::ChannelProxy::MessageFilter {
 public:
  
  DevToolsAgentFilter();

  static void SendRpcMessage(const DevToolsMessageData& data);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 protected:
  virtual ~DevToolsAgentFilter();

 private:
  void OnDispatchOnInspectorBackend(const std::string& message);

  bool message_handled_;
  base::MessageLoop* render_thread_loop_;
  int current_routing_id_;

  DISALLOW_COPY_AND_ASSIGN(DevToolsAgentFilter);
};

}  

#endif  
