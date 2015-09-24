// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_GPU_GPU_CHILD_THREAD_H_
#define CONTENT_GPU_GPU_CHILD_THREAD_H_

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "content/child/child_thread.h"
#include "content/common/gpu/gpu_channel.h"
#include "content/common/gpu/gpu_channel_manager.h"
#include "content/common/gpu/gpu_config.h"
#include "content/common/gpu/x_util.h"
#include "gpu/config/gpu_info.h"
#include "ui/gfx/native_widget_types.h"

namespace sandbox {
class TargetServices;
}

namespace content {
class GpuWatchdogThread;

class GpuChildThread : public ChildThread {
 public:
  typedef std::queue<IPC::Message*> DeferredMessages;

  explicit GpuChildThread(GpuWatchdogThread* gpu_watchdog_thread,
                          bool dead_on_arrival,
                          const gpu::GPUInfo& gpu_info,
                          const DeferredMessages& deferred_messages);

  
  explicit GpuChildThread(const std::string& channel_id);

  virtual ~GpuChildThread();

  virtual void Shutdown() OVERRIDE;

  void Init(const base::Time& process_start_time);
  void StopWatchdog();

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;
  virtual bool OnControlMessageReceived(const IPC::Message& msg) OVERRIDE;

 private:
  
  void OnInitialize();
  void OnCollectGraphicsInfo();
  void OnGetVideoMemoryUsageStats();
  void OnSetVideoMemoryWindowCount(uint32 window_count);

  void OnClean();
  void OnCrash();
  void OnHang();
  void OnDisableWatchdog();

#if defined(USE_TCMALLOC)
  void OnGetGpuTcmalloc();
#endif

  
  
  bool dead_on_arrival_;
  base::Time process_start_time_;
  scoped_refptr<GpuWatchdogThread> watchdog_thread_;

#if defined(OS_WIN)
  
  sandbox::TargetServices* target_services_;
#endif

  scoped_ptr<GpuChannelManager> gpu_channel_manager_;

  
  gpu::GPUInfo gpu_info_;

  
  DeferredMessages deferred_messages_;

  
  bool in_browser_process_;

  DISALLOW_COPY_AND_ASSIGN(GpuChildThread);
};

}  

#endif  
