// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_RENDERER_RESOURCE_H_
#define CHROME_BROWSER_TASK_MANAGER_RENDERER_RESOURCE_H_

#include "base/basictypes.h"
#include "chrome/browser/task_manager/resource_provider.h"

namespace content {
class RenderViewHost;
}

namespace task_manager {

class RendererResource : public Resource {
 public:
  RendererResource(base::ProcessHandle process,
                   content::RenderViewHost* render_view_host);
  virtual ~RendererResource();

  
  virtual base::ProcessHandle GetProcess() const OVERRIDE;
  virtual int GetUniqueChildProcessId() const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual int GetRoutingID() const OVERRIDE;

  virtual bool ReportsCacheStats() const OVERRIDE;
  virtual blink::WebCache::ResourceTypeStats GetWebCoreCacheStats() const
      OVERRIDE;
  virtual bool ReportsFPS() const OVERRIDE;
  virtual float GetFPS() const OVERRIDE;
  virtual bool ReportsV8MemoryStats() const OVERRIDE;
  virtual size_t GetV8MemoryAllocated() const OVERRIDE;
  virtual size_t GetV8MemoryUsed() const OVERRIDE;

  
  virtual bool CanInspect() const OVERRIDE;
  virtual void Inspect() const OVERRIDE;

  
  virtual bool SupportNetworkUsage() const OVERRIDE;
  virtual void SetSupportNetworkUsage() OVERRIDE { }

  virtual void Refresh() OVERRIDE;

  virtual void NotifyResourceTypeStats(
      const blink::WebCache::ResourceTypeStats& stats) OVERRIDE;

  virtual void NotifyFPS(float fps) OVERRIDE;

  virtual void NotifyV8HeapStats(size_t v8_memory_allocated,
                                 size_t v8_memory_used) OVERRIDE;

  content::RenderViewHost* render_view_host() const {
    return render_view_host_;
  }

 private:
  base::ProcessHandle process_;
  int pid_;
  int unique_process_id_;

  
  content::RenderViewHost* render_view_host_;
  
  
  blink::WebCache::ResourceTypeStats stats_;
  
  bool pending_stats_update_;

  
  float fps_;
  
  bool pending_fps_update_;

  
  size_t v8_memory_allocated_;
  size_t v8_memory_used_;
  bool pending_v8_memory_allocated_update_;

  DISALLOW_COPY_AND_ASSIGN(RendererResource);
};

}  

#endif  
