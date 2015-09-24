// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_MANAGER_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_MANAGER_H_

#include <string>

#include "base/callback.h"
#include "content/common/content_export.h"

namespace IPC {
class Message;
}

namespace content {

class DevToolsAgentHost;
class DevToolsClientHost;

class CONTENT_EXPORT DevToolsManager {
 public:
  static DevToolsManager* GetInstance();

  virtual ~DevToolsManager() {}

  
  
  virtual bool DispatchOnInspectorBackend(DevToolsClientHost* from,
                                          const std::string& message) = 0;

  
  virtual void CloseAllClientHosts() = 0;

  
  virtual DevToolsAgentHost* GetDevToolsAgentHostFor(
      DevToolsClientHost* client_host) = 0;

  
  
  
  virtual void RegisterDevToolsClientHostFor(
      DevToolsAgentHost* agent_host,
      DevToolsClientHost* client_host) = 0;

  
  
  
  virtual void ClientHostClosing(DevToolsClientHost* client_host) = 0;

  typedef base::Callback<void(DevToolsAgentHost*, bool attached)> Callback;

  virtual void AddAgentStateCallback(const Callback& callback) = 0;
  virtual void RemoveAgentStateCallback(const Callback& callback) = 0;
};

}  

#endif  
