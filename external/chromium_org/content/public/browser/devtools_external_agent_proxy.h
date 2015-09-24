// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_EXTERNAL_AGENT_PROXY_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_EXTERNAL_AGENT_PROXY_H_

#include <string>

#include "content/common/content_export.h"

namespace content {

class DevToolsAgentHost;
class DevToolsExternalAgentProxyDelegate;

class DevToolsExternalAgentProxy {
 public:
  
  
  
  static CONTENT_EXPORT DevToolsExternalAgentProxy* Create(
      DevToolsExternalAgentProxyDelegate* delegate);

  
  virtual scoped_refptr<DevToolsAgentHost> GetAgentHost() = 0;

  
  virtual void DispatchOnClientHost(const std::string& message) = 0;

  
  virtual void ConnectionClosed() = 0;

  virtual ~DevToolsExternalAgentProxy() {}
};

}  

#endif  
