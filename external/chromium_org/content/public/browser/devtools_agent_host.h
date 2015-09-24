// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_AGENT_HOST_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_AGENT_HOST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace content {

class RenderViewHost;
class WebContents;

class CONTENT_EXPORT DevToolsAgentHost
    : public base::RefCounted<DevToolsAgentHost> {
 public:
  
  static scoped_refptr<DevToolsAgentHost> GetForId(const std::string& id);

  
  
  static scoped_refptr<DevToolsAgentHost> GetOrCreateFor(RenderViewHost* rvh);

  
  
  static bool HasFor(RenderViewHost* rvh);

  
  
  static scoped_refptr<DevToolsAgentHost> GetForWorker(int worker_process_id,
                                                       int worker_route_id);

  
  
  static bool HasForWorker(int worker_process_id,
                           int worker_route_id);

  static bool IsDebuggerAttached(WebContents* web_contents);

  
  static std::vector<RenderViewHost*> GetValidRenderViewHosts();

  
  virtual bool IsAttached() = 0;

  
  virtual void InspectElement(int x, int y) = 0;

  
  virtual std::string GetId() = 0;

  
  virtual RenderViewHost* GetRenderViewHost() = 0;

  
  
  virtual void DisconnectRenderViewHost() = 0;

  
  virtual void ConnectRenderViewHost(RenderViewHost* rvh) = 0;

 protected:
  friend class base::RefCounted<DevToolsAgentHost>;
  virtual ~DevToolsAgentHost() {}
};

}  

#endif  
