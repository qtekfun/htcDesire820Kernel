// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GPU_GPU_PROCESS_HOST_H_
#define CONTENT_BROWSER_GPU_GPU_PROCESS_HOST_H_

#include <map>
#include <queue>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "content/browser/gpu/gpu_surface_tracker.h"
#include "content/common/content_export.h"
#include "content/common/gpu/gpu_memory_uma_stats.h"
#include "content/common/gpu/gpu_process_launch_causes.h"
#include "content/public/browser/browser_child_process_host_delegate.h"
#include "content/public/browser/gpu_data_manager.h"
#include "gpu/command_buffer/common/constants.h"
#include "gpu/config/gpu_info.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sender.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

struct GPUCreateCommandBufferConfig;
struct GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params;
struct GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params;
struct GpuHostMsg_AcceleratedSurfaceRelease_Params;

namespace IPC {
struct ChannelHandle;
}

namespace content {
class BrowserChildProcessHostImpl;
class GpuMainThread;
class RenderWidgetHostViewFrameSubscriber;
class ShaderDiskCache;

typedef base::Thread* (*GpuMainThreadFactoryFunction)(const std::string& id);

class GpuProcessHost : public BrowserChildProcessHostDelegate,
                       public IPC::Sender,
                       public base::NonThreadSafe {
 public:
  enum GpuProcessKind {
    GPU_PROCESS_KIND_UNSANDBOXED,
    GPU_PROCESS_KIND_SANDBOXED,
    GPU_PROCESS_KIND_COUNT
  };

  typedef base::Callback<void(const IPC::ChannelHandle&, const gpu::GPUInfo&)>
      EstablishChannelCallback;

  typedef base::Callback<void(int32)> CreateCommandBufferCallback;

  typedef base::Callback<void(const gfx::Size)> CreateImageCallback;

  static bool gpu_enabled() { return gpu_enabled_; }

  
  
  
  
  
  CONTENT_EXPORT static GpuProcessHost* Get(GpuProcessKind kind,
                                            CauseForGpuLaunch cause);

  
  static void GetProcessHandles(
      const GpuDataManager::GetGpuProcessHandlesCallback& callback);

  
  
  
  CONTENT_EXPORT static void SendOnIO(GpuProcessKind kind,
                                      CauseForGpuLaunch cause,
                                      IPC::Message* message);

  CONTENT_EXPORT static void RegisterGpuMainThreadFactory(
      GpuMainThreadFactoryFunction create);

  
  
  static GpuProcessHost* FromID(int host_id);
  int host_id() const { return host_id_; }

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  void AddFilter(IPC::ChannelProxy::MessageFilter* filter);

  
  
  
  void EstablishGpuChannel(int client_id,
                           bool share_context,
                           const EstablishChannelCallback& callback);

  
  
  void CreateViewCommandBuffer(
      const gfx::GLSurfaceHandle& compositing_surface,
      int surface_id,
      int client_id,
      const GPUCreateCommandBufferConfig& init_params,
      const CreateCommandBufferCallback& callback);

  
  void CreateImage(
      gfx::PluginWindowHandle window,
      int client_id,
      int image_id,
      const CreateImageCallback& callback);

    
  void DeleteImage(int client_id, int image_id, int sync_point);

  
  GpuProcessKind kind();

  void ForceShutdown();

  void BeginFrameSubscription(
      int surface_id,
      base::WeakPtr<RenderWidgetHostViewFrameSubscriber> subscriber);
  void EndFrameSubscription(int surface_id);
  void LoadedShader(const std::string& key, const std::string& data);

 private:
  static bool ValidateHost(GpuProcessHost* host);

  GpuProcessHost(int host_id, GpuProcessKind kind);
  virtual ~GpuProcessHost();

  bool Init();

  
  void RouteOnUIThread(const IPC::Message& message);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnProcessLaunched() OVERRIDE;
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;

  
  void OnInitialized(bool result, const gpu::GPUInfo& gpu_info);
  void OnChannelEstablished(const IPC::ChannelHandle& channel_handle);
  void OnCommandBufferCreated(const int32 route_id);
  void OnDestroyCommandBuffer(int32 surface_id);
  void OnImageCreated(const gfx::Size size);
  void OnDidCreateOffscreenContext(const GURL& url);
  void OnDidLoseContext(bool offscreen,
                        gpu::error::ContextLostReason reason,
                        const GURL& url);
  void OnDidDestroyOffscreenContext(const GURL& url);
  void OnGpuMemoryUmaStatsReceived(const GPUMemoryUmaStats& stats);
#if defined(OS_MACOSX)
  void OnAcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params);
#endif
  
#if defined(OS_WIN)
  void OnAcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params);
  void OnAcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params);
  void OnAcceleratedSurfaceSuspend(int32 surface_id);
  void OnAcceleratedSurfaceRelease(
    const GpuHostMsg_AcceleratedSurfaceRelease_Params& params);
#endif

  void CreateChannelCache(int32 client_id);
  void OnDestroyChannel(int32 client_id);
  void OnCacheShader(int32 client_id, const std::string& key,
                     const std::string& shader);

  bool LaunchGpuProcess(const std::string& channel_id);

  void SendOutstandingReplies();

  void BlockLiveOffscreenContexts();

  std::string GetShaderPrefixKey();

  
  int host_id_;

  
  
  std::queue<EstablishChannelCallback> channel_requests_;

  
  std::queue<CreateCommandBufferCallback> create_command_buffer_requests_;

  
  std::queue<CreateImageCallback> create_image_requests_;


  
  std::queue<IPC::Message*> queued_messages_;

  
  bool valid_;

  
  
  bool in_process_;

  bool swiftshader_rendering_;
  GpuProcessKind kind_;

  scoped_ptr<base::Thread> in_process_gpu_thread_;

  
  bool process_launched_;

  
  bool initialized_;

  
  base::TimeTicks init_start_time_;

  
  
  
  static bool gpu_enabled_;

  static bool hardware_gpu_enabled_;

  scoped_ptr<BrowserChildProcessHostImpl> process_;

  
  
  
  
  
  std::multiset<GURL> urls_with_live_offscreen_contexts_;

  
  bool uma_memory_stats_received_;
  GPUMemoryUmaStats uma_memory_stats_;

  
  
  typedef base::hash_map<int,
                         base::WeakPtr<RenderWidgetHostViewFrameSubscriber> >
  FrameSubscriberMap;
  FrameSubscriberMap frame_subscribers_;

  typedef std::map<int32, scoped_refptr<ShaderDiskCache> >
      ClientIdToShaderCacheMap;
  ClientIdToShaderCacheMap client_id_to_shader_cache_;

  std::string shader_prefix_key_;

  
  
  
  typedef std::multimap<int, scoped_refptr<GpuSurfaceTracker::SurfaceRef> >
      SurfaceRefMap;
  SurfaceRefMap surface_refs_;

  DISALLOW_COPY_AND_ASSIGN(GpuProcessHost);
};

}  

#endif  
