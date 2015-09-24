// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GPU_GPU_PROCESS_HOST_UI_SHIM_H_
#define CONTENT_BROWSER_GPU_GPU_PROCESS_HOST_UI_SHIM_H_


#include <string>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"
#include "content/common/message_router.h"
#include "content/public/common/gpu_memory_stats.h"
#include "gpu/config/gpu_info.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"

struct GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params;
struct GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params;
struct GpuHostMsg_AcceleratedSurfaceRelease_Params;

namespace ui {
struct LatencyInfo;
}

namespace gfx {
class Size;
}

namespace IPC {
class Message;
}

namespace content {
void RouteToGpuProcessHostUIShimTask(int host_id, const IPC::Message& msg);

class GpuProcessHostUIShim : public IPC::Listener,
                             public IPC::Sender,
                             public base::NonThreadSafe {
 public:
  
  
  static GpuProcessHostUIShim* Create(int host_id);

  
  
  
  static void Destroy(int host_id, const std::string& message);

  
  CONTENT_EXPORT static void DestroyAll();

  CONTENT_EXPORT static GpuProcessHostUIShim* FromID(int host_id);

  
  
  CONTENT_EXPORT static GpuProcessHostUIShim* GetOneInstance();

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  
  
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  CONTENT_EXPORT void SimulateRemoveAllContext();
  CONTENT_EXPORT void SimulateCrash();
  CONTENT_EXPORT void SimulateHang();

 private:
  explicit GpuProcessHostUIShim(int host_id);
  virtual ~GpuProcessHostUIShim();

  
  bool OnControlMessageReceived(const IPC::Message& message);

  void OnLogMessage(int level, const std::string& header,
      const std::string& message);
  void OnResizeView(int32 surface_id,
                    int32 route_id,
                    gfx::Size size);

  void OnGraphicsInfoCollected(const gpu::GPUInfo& gpu_info);

  void OnAcceleratedSurfaceInitialized(int32 surface_id, int32 route_id);
  void OnAcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params);
  void OnAcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params);
  void OnAcceleratedSurfaceSuspend(int32 surface_id);
  void OnAcceleratedSurfaceRelease(
      const GpuHostMsg_AcceleratedSurfaceRelease_Params& params);
  void OnVideoMemoryUsageStatsReceived(
      const GPUVideoMemoryUsageStats& video_memory_usage_stats);
  void OnUpdateVSyncParameters(int surface_id,
                               base::TimeTicks timebase,
                               base::TimeDelta interval);
  void OnFrameDrawn(const ui::LatencyInfo& latency_info);

  
  int host_id_;
};

}  

#endif  
