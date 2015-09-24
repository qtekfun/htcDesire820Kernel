// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_GPU_COMMAND_BUFFER_STUB_H_
#define CONTENT_COMMON_GPU_GPU_COMMAND_BUFFER_STUB_H_

#include <deque>
#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "content/common/content_export.h"
#include "content/common/gpu/gpu_memory_manager.h"
#include "content/common/gpu/gpu_memory_manager_client.h"
#include "gpu/command_buffer/common/constants.h"
#include "gpu/command_buffer/common/gpu_memory_allocation.h"
#include "gpu/command_buffer/service/command_buffer_service.h"
#include "gpu/command_buffer/service/context_group.h"
#include "gpu/command_buffer/service/gpu_scheduler.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "media/base/video_decoder_config.h"
#include "ui/events/latency_info.h"
#include "ui/gfx/gpu_memory_buffer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gpu_preference.h"
#include "url/gurl.h"

namespace gpu {
class GpuControlService;
struct Mailbox;
namespace gles2 {
class ImageManager;
class MailboxManager;
}
}

namespace content {

class GpuChannel;
class GpuVideoDecodeAccelerator;
class GpuWatchdog;

class GpuCommandBufferStub
    : public GpuMemoryManagerClient,
      public IPC::Listener,
      public IPC::Sender,
      public base::SupportsWeakPtr<GpuCommandBufferStub> {
 public:
  class DestructionObserver {
   public:
    
    virtual void OnWillDestroyStub() = 0;

   protected:
    virtual ~DestructionObserver() {}
  };

  typedef base::Callback<void(const ui::LatencyInfo&)>
      LatencyInfoCallback;

  GpuCommandBufferStub(
      GpuChannel* channel,
      GpuCommandBufferStub* share_group,
      const gfx::GLSurfaceHandle& handle,
      gpu::gles2::MailboxManager* mailbox_manager,
      gpu::gles2::ImageManager* image_manager,
      const gfx::Size& size,
      const gpu::gles2::DisallowedFeatures& disallowed_features,
      const std::vector<int32>& attribs,
      gfx::GpuPreference gpu_preference,
      bool use_virtualized_gl_context,
      int32 route_id,
      int32 surface_id,
      GpuWatchdog* watchdog,
      bool software,
      const GURL& active_url);

  virtual ~GpuCommandBufferStub();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual gfx::Size GetSurfaceSize() const OVERRIDE;
  virtual gpu::gles2::MemoryTracker* GetMemoryTracker() const OVERRIDE;
  virtual void SetMemoryAllocation(
      const gpu::MemoryAllocation& allocation) OVERRIDE;
  virtual void SuggestHaveFrontBuffer(bool suggest_have_frontbuffer) OVERRIDE;
  virtual bool GetTotalGpuMemory(uint64* bytes) OVERRIDE;

  
  bool IsScheduled();

  
  bool IsPreempted() const {
    return scheduler_.get() && scheduler_->IsPreempted();
  }

  
  bool HasUnprocessedCommands();

  gpu::gles2::GLES2Decoder* decoder() const { return decoder_.get(); }
  gpu::GpuScheduler* scheduler() const { return scheduler_.get(); }
  GpuChannel* channel() const { return channel_; }

  
  int32 surface_id() const { return surface_id_; }

  
  
  int32 route_id() const { return route_id_; }

  gfx::GpuPreference gpu_preference() { return gpu_preference_; }

  int32 GetRequestedAttribute(int attr) const;

  
  void SendConsoleMessage(int32 id, const std::string& message);

  void SendCachedShader(const std::string& key, const std::string& shader);

  gfx::GLSurface* surface() const { return surface_.get(); }

  void AddDestructionObserver(DestructionObserver* observer);
  void RemoveDestructionObserver(DestructionObserver* observer);

  
  
  void AddSyncPoint(uint32 sync_point);

  void SetPreemptByFlag(scoped_refptr<gpu::PreemptionFlag> flag);

  void SetLatencyInfoCallback(const LatencyInfoCallback& callback);

  void MarkContextLost();

  uint64 GetMemoryUsage() const;

 private:
  GpuMemoryManager* GetMemoryManager() const;
  bool MakeCurrent();
  void Destroy();

  
  void OnInitializeFailed(IPC::Message* reply_message);

  
  void OnInitialize(base::SharedMemoryHandle shared_state_shm,
                    IPC::Message* reply_message);
  void OnSetGetBuffer(int32 shm_id, IPC::Message* reply_message);
  void OnProduceFrontBuffer(const gpu::Mailbox& mailbox);
  void OnGetState(IPC::Message* reply_message);
  void OnGetStateFast(IPC::Message* reply_message);
  void OnAsyncFlush(int32 put_offset, uint32 flush_count);
  void OnEcho(const IPC::Message& message);
  void OnRescheduled();
  void OnRegisterTransferBuffer(int32 id,
                                base::SharedMemoryHandle transfer_buffer,
                                uint32 size);
  void OnDestroyTransferBuffer(int32 id);
  void OnGetTransferBuffer(int32 id, IPC::Message* reply_message);

  void OnCreateVideoDecoder(
      media::VideoCodecProfile profile,
      IPC::Message* reply_message);

  void OnSetSurfaceVisible(bool visible);

  void OnEnsureBackbuffer();

  void OnRetireSyncPoint(uint32 sync_point);
  bool OnWaitSyncPoint(uint32 sync_point);
  void OnSyncPointRetired();
  void OnSignalSyncPoint(uint32 sync_point, uint32 id);
  void OnSignalSyncPointAck(uint32 id);
  void OnSignalQuery(uint32 query, uint32 id);

  void OnReceivedClientManagedMemoryStats(const gpu::ManagedMemoryStats& stats);
  void OnSetClientHasMemoryAllocationChangedCallback(bool has_callback);

  void OnRegisterGpuMemoryBuffer(int32 id,
                                 gfx::GpuMemoryBufferHandle gpu_memory_buffer,
                                 uint32 width,
                                 uint32 height,
                                 uint32 internalformat);
  void OnDestroyGpuMemoryBuffer(int32 id);

  void OnCommandProcessed();
  void OnParseError();
  void OnSetLatencyInfo(const ui::LatencyInfo& latency_info);

  void ReportState();

  
  void PutChanged();

  
  void PollWork();

  
  bool HasMoreWork();

  void ScheduleDelayedWork(int64 delay);

  bool CheckContextLost();

  
  
  
  GpuChannel* channel_;

  
  scoped_refptr<gpu::gles2::ContextGroup> context_group_;

  gfx::GLSurfaceHandle handle_;
  gfx::Size initial_size_;
  gpu::gles2::DisallowedFeatures disallowed_features_;
  std::vector<int32> requested_attribs_;
  gfx::GpuPreference gpu_preference_;
  bool use_virtualized_gl_context_;
  int32 route_id_;
  int32 surface_id_;
  bool software_;
  uint32 last_flush_count_;

  scoped_ptr<gpu::CommandBufferService> command_buffer_;
  scoped_ptr<gpu::gles2::GLES2Decoder> decoder_;
  scoped_ptr<gpu::GpuScheduler> scheduler_;
  scoped_refptr<gfx::GLSurface> surface_;
  scoped_ptr<gpu::GpuControlService> gpu_control_;

  scoped_ptr<GpuMemoryManagerClientState> memory_manager_client_state_;
  
  
  bool last_memory_allocation_valid_;
  gpu::MemoryAllocation last_memory_allocation_;

  GpuWatchdog* watchdog_;

  ObserverList<DestructionObserver> destruction_observers_;

  
  std::deque<uint32> sync_points_;
  int sync_point_wait_count_;

  bool delayed_work_scheduled_;
  uint64 previous_messages_processed_;
  base::TimeTicks last_idle_time_;

  scoped_refptr<gpu::PreemptionFlag> preemption_flag_;

  LatencyInfoCallback latency_info_callback_;

  GURL active_url_;
  size_t active_url_hash_;

  size_t total_gpu_memory_;

  DISALLOW_COPY_AND_ASSIGN(GpuCommandBufferStub);
};

}  

#endif  
