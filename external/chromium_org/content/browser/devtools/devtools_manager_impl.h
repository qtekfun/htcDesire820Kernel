// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_MANAGER_IMPL_H_
#define CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_MANAGER_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "content/browser/devtools/devtools_agent_host_impl.h"
#include "content/common/content_export.h"
#include "content/public/browser/devtools_client_host.h"
#include "content/public/browser/devtools_manager.h"

class GURL;

namespace IPC {
class Message;
}

namespace content {

class RenderViewHost;

class CONTENT_EXPORT DevToolsManagerImpl
    : public DevToolsAgentHostImpl::CloseListener,
      public DevToolsManager {
 public:
  
  
  static DevToolsManagerImpl* GetInstance();

  DevToolsManagerImpl();
  virtual ~DevToolsManagerImpl();

  void DispatchOnInspectorFrontend(DevToolsAgentHost* agent_host,
                                   const std::string& message);

  
  virtual bool DispatchOnInspectorBackend(DevToolsClientHost* from,
                                          const std::string& message) OVERRIDE;
  virtual void CloseAllClientHosts() OVERRIDE;
  virtual DevToolsAgentHost* GetDevToolsAgentHostFor(
      DevToolsClientHost* client_host) OVERRIDE;
  virtual void RegisterDevToolsClientHostFor(
      DevToolsAgentHost* agent_host,
      DevToolsClientHost* client_host) OVERRIDE;
  virtual void ClientHostClosing(DevToolsClientHost* host) OVERRIDE;
  virtual void AddAgentStateCallback(const Callback& callback) OVERRIDE;
  virtual void RemoveAgentStateCallback(const Callback& callback) OVERRIDE;

 private:
  friend class DevToolsAgentHostImpl;
  friend class RenderViewDevToolsAgentHost;
  friend struct DefaultSingletonTraits<DevToolsManagerImpl>;

  
  virtual void AgentHostClosing(DevToolsAgentHostImpl* host) OVERRIDE;

  void BindClientHost(DevToolsAgentHostImpl* agent_host,
                      DevToolsClientHost* client_host);
  void UnbindClientHost(DevToolsAgentHostImpl* agent_host,
                        DevToolsClientHost* client_host);

  DevToolsClientHost* GetDevToolsClientHostFor(
      DevToolsAgentHostImpl* agent_host);

  void UnregisterDevToolsClientHostFor(DevToolsAgentHostImpl* agent_host);

  void NotifyObservers(DevToolsAgentHost* agent_host, bool attached);

  
  
  
  
  
  
  typedef std::map<DevToolsAgentHostImpl*, DevToolsClientHost*>
      AgentToClientHostMap;
  AgentToClientHostMap agent_to_client_host_;

  typedef std::map<DevToolsClientHost*, scoped_refptr<DevToolsAgentHostImpl> >
      ClientToAgentHostMap;
  ClientToAgentHostMap client_to_agent_host_;

  typedef std::vector<const Callback*> CallbackContainer;
  CallbackContainer callbacks_;

  DISALLOW_COPY_AND_ASSIGN(DevToolsManagerImpl);
};

}  

#endif  
