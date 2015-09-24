// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GPU_GPU_DATA_MANAGER_IMPL_H_
#define CONTENT_BROWSER_GPU_GPU_DATA_MANAGER_IMPL_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/process/kill.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "base/values.h"
#include "content/public/browser/gpu_data_manager.h"
#include "content/public/common/gpu_memory_stats.h"
#include "content/public/common/three_d_api_types.h"
#include "gpu/config/gpu_info.h"

class CommandLine;
class GURL;
struct WebPreferences;

namespace content {

class GpuDataManagerImplPrivate;

class CONTENT_EXPORT GpuDataManagerImpl
    : public NON_EXPORTED_BASE(GpuDataManager) {
 public:
  
  
  
  
  enum DomainGuilt {
    DOMAIN_GUILT_KNOWN,
    DOMAIN_GUILT_UNKNOWN
  };

  
  
  
  enum DomainBlockStatus {
    DOMAIN_BLOCK_STATUS_BLOCKED,
    DOMAIN_BLOCK_STATUS_ALL_DOMAINS_BLOCKED,
    DOMAIN_BLOCK_STATUS_NOT_BLOCKED
  };

  
  static GpuDataManagerImpl* GetInstance();

  
  virtual void InitializeForTesting(
      const std::string& gpu_blacklist_json,
      const gpu::GPUInfo& gpu_info) OVERRIDE;
  virtual bool IsFeatureBlacklisted(int feature) const OVERRIDE;
  virtual gpu::GPUInfo GetGPUInfo() const OVERRIDE;
  virtual void GetGpuProcessHandles(
      const GetGpuProcessHandlesCallback& callback) const OVERRIDE;
  virtual bool GpuAccessAllowed(std::string* reason) const OVERRIDE;
  virtual void RequestCompleteGpuInfoIfNeeded() OVERRIDE;
  virtual bool IsCompleteGpuInfoAvailable() const OVERRIDE;
  virtual void RequestVideoMemoryUsageStatsUpdate() const OVERRIDE;
  virtual bool ShouldUseSwiftShader() const OVERRIDE;
  virtual void RegisterSwiftShaderPath(const base::FilePath& path) OVERRIDE;
  virtual void AddObserver(GpuDataManagerObserver* observer) OVERRIDE;
  virtual void RemoveObserver(GpuDataManagerObserver* observer) OVERRIDE;
  virtual void UnblockDomainFrom3DAPIs(const GURL& url) OVERRIDE;
  virtual void DisableGpuWatchdog() OVERRIDE;
  virtual void SetGLStrings(const std::string& gl_vendor,
                            const std::string& gl_renderer,
                            const std::string& gl_version) OVERRIDE;
  virtual void GetGLStrings(std::string* gl_vendor,
                            std::string* gl_renderer,
                            std::string* gl_version) OVERRIDE;
  virtual void DisableHardwareAcceleration() OVERRIDE;
  virtual bool CanUseGpuBrowserCompositor() const OVERRIDE;

  
  
  
  void Initialize();

  
  
  void UpdateGpuInfo(const gpu::GPUInfo& gpu_info);

  void UpdateVideoMemoryUsageStats(
      const GPUVideoMemoryUsageStats& video_memory_usage_stats);

  
  
  void AppendRendererCommandLine(CommandLine* command_line) const;

  
  
  void AppendGpuCommandLine(CommandLine* command_line) const;

  
  
  void AppendPluginCommandLine(CommandLine* command_line) const;

  
  void UpdateRendererWebPrefs(WebPreferences* prefs) const;

  std::string GetBlacklistVersion() const;
  std::string GetDriverBugListVersion() const;

  
  
  
  void GetBlacklistReasons(base::ListValue* reasons) const;

  
  
  void GetDriverBugWorkarounds(base::ListValue* workarounds) const;

  void AddLogMessage(int level,
                     const std::string& header,
                     const std::string& message);

  void ProcessCrashed(base::TerminationStatus exit_code);

  
  
  base::ListValue* GetLogMessages() const;

  
  void HandleGpuSwitch();

#if defined(OS_WIN)
  
  
  bool IsUsingAcceleratedSurface() const;
#endif

  
  
  
  
  
  
  
  
  
  
  
  void BlockDomainFrom3DAPIs(const GURL& url, DomainGuilt guilt);
  bool Are3DAPIsBlocked(const GURL& url,
                        int render_process_id,
                        int render_view_id,
                        ThreeDAPIType requester);

  
  void DisableDomainBlockingFor3DAPIsForTesting();

  void Notify3DAPIBlocked(const GURL& url,
                          int render_process_id,
                          int render_view_id,
                          ThreeDAPIType requester);

  
  size_t GetBlacklistedFeatureCount() const;

  void SetDisplayCount(unsigned int display_count);
  unsigned int GetDisplayCount() const;

  
  void OnGpuProcessInitFailure();

  bool IsDriverBugWorkaroundActive(int feature) const;

 private:
  friend class GpuDataManagerImplPrivate;
  friend class GpuDataManagerImplPrivateTest;
  friend struct DefaultSingletonTraits<GpuDataManagerImpl>;

  
  
  
  
  
  class UnlockedSession {
   public:
    explicit UnlockedSession(GpuDataManagerImpl* owner)
        : owner_(owner) {
      DCHECK(owner_);
      owner_->lock_.AssertAcquired();
      owner_->lock_.Release();
    }

    ~UnlockedSession() {
      DCHECK(owner_);
      owner_->lock_.Acquire();
    }

   private:
    GpuDataManagerImpl* owner_;
    DISALLOW_COPY_AND_ASSIGN(UnlockedSession);
  };

  GpuDataManagerImpl();
  virtual ~GpuDataManagerImpl();

  mutable base::Lock lock_;
  scoped_ptr<GpuDataManagerImplPrivate> private_;

  DISALLOW_COPY_AND_ASSIGN(GpuDataManagerImpl);
};

}  

#endif  
