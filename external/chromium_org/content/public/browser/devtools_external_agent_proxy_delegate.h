// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_EXTERNAL_AGENT_PROXY_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_EXTERNAL_AGENT_PROXY_DELEGATE_H_

#include <string>

#include "content/common/content_export.h"

namespace content {

class DevToolsExternalAgentProxyDelegate {
 public:
   
   virtual void Attach() = 0;

   
   virtual void Detach() = 0;

   
   virtual void SendMessageToBackend(const std::string& message) = 0;

 protected:
  virtual ~DevToolsExternalAgentProxyDelegate() {}
};

}  

#endif  
