// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_GPU_DATA_MANAGER_OBSERVER_H_
#define CONTENT_PUBLIC_BROWSER_GPU_DATA_MANAGER_OBSERVER_H_

#include "base/process/kill.h"
#include "content/common/content_export.h"
#include "content/public/common/gpu_memory_stats.h"
#include "content/public/common/three_d_api_types.h"

class GURL;

namespace content {

class CONTENT_EXPORT GpuDataManagerObserver {
 public:
  
  virtual void OnGpuInfoUpdate() {}

  
  virtual void OnVideoMemoryUsageStatsUpdate(
      const GPUVideoMemoryUsageStats& video_memory_usage_stats) {}

  
  
  virtual void DidBlock3DAPIs(const GURL& url,
                              int render_process_id,
                              int render_view_id,
                              ThreeDAPIType requester) {}

  
  virtual void OnGpuProcessCrashed(base::TerminationStatus exit_code) {}

  
  virtual void OnGpuSwitching() {}

 protected:
  virtual ~GpuDataManagerObserver() {}
};

};  

#endif  
