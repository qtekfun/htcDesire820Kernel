// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_GPU_DATA_MANAGER_H_
#define CONTENT_PUBLIC_BROWSER_GPU_DATA_MANAGER_H_

#include <list>
#include <string>

#include "base/callback_forward.h"
#include "base/process/process.h"
#include "content/common/content_export.h"

class GURL;

namespace base {
class FilePath;
class ListValue;
}

namespace gpu {
struct GPUInfo;
}

namespace content {

class GpuDataManagerObserver;

class GpuDataManager {
 public:
  typedef base::Callback<void(const std::list<base::ProcessHandle>&)>
      GetGpuProcessHandlesCallback;

  
  CONTENT_EXPORT static GpuDataManager* GetInstance();

  virtual void InitializeForTesting(const std::string& gpu_blacklist_json,
                                    const gpu::GPUInfo& gpu_info) = 0;

  virtual bool IsFeatureBlacklisted(int feature) const = 0;

  virtual gpu::GPUInfo GetGPUInfo() const = 0;

  
  virtual void GetGpuProcessHandles(
      const GetGpuProcessHandlesCallback& callback) const = 0;

  
  
  
  
  
  
  
  
  virtual bool GpuAccessAllowed(std::string* reason) const = 0;

  
  virtual void RequestCompleteGpuInfoIfNeeded() = 0;

  virtual bool IsCompleteGpuInfoAvailable() const = 0;

  
  
  virtual void RequestVideoMemoryUsageStatsUpdate() const = 0;

  
  virtual bool ShouldUseSwiftShader() const = 0;

  
  virtual void RegisterSwiftShaderPath(const base::FilePath& path) = 0;

  
  virtual void AddObserver(GpuDataManagerObserver* observer) = 0;
  virtual void RemoveObserver(GpuDataManagerObserver* observer) = 0;

  
  
  virtual void UnblockDomainFrom3DAPIs(const GURL& url) = 0;

  
  virtual void DisableGpuWatchdog() = 0;

  
  
  virtual void SetGLStrings(const std::string& gl_vendor,
                            const std::string& gl_renderer,
                            const std::string& gl_version) = 0;

  
  virtual void GetGLStrings(std::string* gl_vendor,
                            std::string* gl_renderer,
                            std::string* gl_version) = 0;

  
  virtual void DisableHardwareAcceleration() = 0;

  
  virtual bool CanUseGpuBrowserCompositor() const = 0;

 protected:
  virtual ~GpuDataManager() {}
};

};  

#endif  
