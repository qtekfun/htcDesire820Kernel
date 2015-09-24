// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_AGENT_HOST_IMPL_H_
#define CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_AGENT_HOST_IMPL_H_

#include <string>

#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "content/public/browser/devtools_agent_host.h"

namespace IPC {
class Message;
}

namespace content {

class CONTENT_EXPORT DevToolsAgentHostImpl : public DevToolsAgentHost {
 public:
  class CONTENT_EXPORT CloseListener {
   public:
    virtual void AgentHostClosing(DevToolsAgentHostImpl*) = 0;
   protected:
    virtual ~CloseListener() {}
  };

  
  virtual void Attach() = 0;

  
  virtual void Detach() = 0;

  
  virtual void DispatchOnInspectorBackend(const std::string& message) = 0;

  void set_close_listener(CloseListener* listener) {
    close_listener_ = listener;
  }

  
  virtual bool IsAttached() OVERRIDE;

  virtual void InspectElement(int x, int y) OVERRIDE;

  virtual std::string GetId() OVERRIDE;

  virtual RenderViewHost* GetRenderViewHost() OVERRIDE;

  virtual void DisconnectRenderViewHost() OVERRIDE;

  virtual void ConnectRenderViewHost(RenderViewHost* rvh) OVERRIDE;

 protected:
  DevToolsAgentHostImpl();
  virtual ~DevToolsAgentHostImpl();

  void NotifyCloseListener();

 private:
  CloseListener* close_listener_;
  const std::string id_;
};

}  

#endif  
