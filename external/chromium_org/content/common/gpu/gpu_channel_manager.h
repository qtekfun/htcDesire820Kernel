// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_GPU_CHANNEL_MANAGER_H_
#define CONTENT_COMMON_GPU_GPU_CHANNEL_MANAGER_H_

#include <deque>
#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "build/build_config.h"
#include "content/common/gpu/devtools_gpu_instrumentation.h"
#include "content/common/gpu/gpu_memory_manager.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gl/gl_surface.h"

namespace base {
class WaitableEvent;
}

namespace gfx {
class GLShareGroup;
}

namespace gpu {
namespace gles2 {
class MailboxManager;
class ProgramCache;
}
}

namespace IPC {
struct ChannelHandle;
}

struct GPUCreateCommandBufferConfig;

namespace content {
class ChildThread;
class GpuChannel;
class GpuWatchdog;
class SyncPointManager;

class GpuChannelManager : public IPC::Listener,
                          public IPC::Sender {
 public:
  GpuChannelManager(ChildThread* gpu_child_thread,
                    GpuWatchdog* watchdog,
                    base::MessageLoopProxy* io_message_loop,
                    base::WaitableEvent* shutdown_event);
  virtual ~GpuChannelManager();

  
  void RemoveChannel(int client_id);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  bool HandleMessagesScheduled();
  uint64 MessagesProcessed();

  void LoseAllContexts();

  base::WeakPtrFactory<GpuChannelManager> weak_factory_;

  int GenerateRouteID();
  void AddRoute(int32 routing_id, IPC::Listener* listener);
  void RemoveRoute(int32 routing_id);

  gpu::gles2::ProgramCache* program_cache();

  GpuMemoryManager* gpu_memory_manager() { return &gpu_memory_manager_; }

  GpuEventsDispatcher* gpu_devtools_events_dispatcher() {
    return &gpu_devtools_events_dispatcher_;
  }

  GpuChannel* LookupChannel(int32 client_id);

  SyncPointManager* sync_point_manager() { return sync_point_manager_.get(); }

  gfx::GLSurface* GetDefaultOffscreenSurface();

 private:
  struct ImageOperation {
    ImageOperation(int32 sync_point, base::Closure callback);
    ~ImageOperation();

    int32 sync_point;
    base::Closure callback;
  };
  typedef base::hash_map<int, scoped_refptr<GpuChannel> > GpuChannelMap;
  typedef std::deque<ImageOperation*> ImageOperationQueue;

  
  void OnEstablishChannel(int client_id, bool share_context);
  void OnCloseChannel(const IPC::ChannelHandle& channel_handle);
  void OnVisibilityChanged(
      int32 render_view_id, int32 client_id, bool visible);
  void OnCreateViewCommandBuffer(
      const gfx::GLSurfaceHandle& window,
      int32 render_view_id,
      int32 client_id,
      const GPUCreateCommandBufferConfig& init_params);
  void CreateImage(
      gfx::PluginWindowHandle window, int32 client_id, int32 image_id);
  void OnCreateImage(
      gfx::PluginWindowHandle window, int32 client_id, int32 image_id);
  void DeleteImage(int32 client_id, int32 image_id);
  void OnDeleteImage(int32 client_id, int32 image_id, int32 sync_point);
  void OnDeleteImageSyncPointRetired(ImageOperation*);
  void OnLoadedShader(std::string shader);

  void OnLoseAllContexts();

  scoped_refptr<base::MessageLoopProxy> io_message_loop_;
  base::WaitableEvent* shutdown_event_;

  
  ChildThread* gpu_child_thread_;

  
  
  
  GpuChannelMap gpu_channels_;
  scoped_refptr<gfx::GLShareGroup> share_group_;
  scoped_refptr<gpu::gles2::MailboxManager> mailbox_manager_;
  GpuMemoryManager gpu_memory_manager_;
  GpuEventsDispatcher gpu_devtools_events_dispatcher_;
  GpuWatchdog* watchdog_;
  scoped_refptr<SyncPointManager> sync_point_manager_;
  scoped_ptr<gpu::gles2::ProgramCache> program_cache_;
  scoped_refptr<gfx::GLSurface> default_offscreen_surface_;
  ImageOperationQueue image_operations_;

  DISALLOW_COPY_AND_ASSIGN(GpuChannelManager);
};

}  

#endif  