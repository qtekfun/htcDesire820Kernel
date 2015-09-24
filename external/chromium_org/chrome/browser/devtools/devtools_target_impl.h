// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEVTOOLS_DEVTOOLS_TARGET_IMPL_H_
#define CHROME_BROWSER_DEVTOOLS_DEVTOOLS_TARGET_IMPL_H_

#include <vector>

#include "base/callback.h"
#include "content/public/browser/devtools_target.h"
#include "content/public/browser/worker_service.h"

class Profile;

namespace content {
class DevToolsAgentHost;
class RenderViewHost;
}

class DevToolsTargetImpl : public content::DevToolsTarget {
 public:

  DevToolsTargetImpl();
  virtual ~DevToolsTargetImpl();

  
  virtual std::string GetId() const OVERRIDE;
  virtual std::string GetType() const OVERRIDE;
  virtual std::string GetTitle() const OVERRIDE;
  virtual std::string GetDescription() const OVERRIDE;
  virtual GURL GetUrl() const OVERRIDE;
  virtual GURL GetFaviconUrl() const OVERRIDE;
  virtual base::TimeTicks GetLastActivityTime() const OVERRIDE;
  virtual bool IsAttached() const OVERRIDE;
  virtual scoped_refptr<content::DevToolsAgentHost> GetAgentHost() const
    OVERRIDE;
  virtual bool Activate() const OVERRIDE;
  virtual bool Close() const OVERRIDE;

  
  
  virtual content::RenderViewHost* GetRenderViewHost() const;

  
  virtual int GetTabId() const;

  
  
  virtual std::string GetExtensionId() const;

  
  virtual void Inspect(Profile* profile) const;

  
  virtual void Reload() const;

  
  static scoped_ptr<DevToolsTargetImpl> CreateForRenderViewHost(
      content::RenderViewHost*, bool is_tab);

  
  static scoped_ptr<DevToolsTargetImpl> CreateForWorker(
      const content::WorkerService::WorkerInfo&);

  typedef std::vector<DevToolsTargetImpl*> List;
  typedef base::Callback<void(const List&)> Callback;

  static List EnumerateRenderViewHostTargets();
  static void EnumerateWorkerTargets(Callback callback);
  static void EnumerateAllTargets(Callback callback);

 protected:
  scoped_refptr<content::DevToolsAgentHost> agent_host_;
  std::string id_;
  std::string type_;
  std::string title_;
  std::string description_;
  GURL url_;
  GURL favicon_url_;
  base::TimeTicks last_activity_time_;
};

#endif  
