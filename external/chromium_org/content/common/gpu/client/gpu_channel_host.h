// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_CLIENT_GPU_CHANNEL_HOST_H_
#define CONTENT_COMMON_GPU_CLIENT_GPU_CHANNEL_HOST_H_

#include <string>
#include <vector>

#include "base/atomic_sequence_num.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/process/process.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "content/common/gpu/gpu_process_launch_causes.h"
#include "content/common/message_router.h"
#include "gpu/config/gpu_info.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sync_channel.h"
#include "media/video/video_decode_accelerator.h"
#include "media/video/video_encode_accelerator.h"
#include "ui/gfx/gpu_memory_buffer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gl/gpu_preference.h"

class GURL;
class TransportTextureService;
struct GPUCreateCommandBufferConfig;

namespace base {
class MessageLoop;
class MessageLoopProxy;
}

namespace gpu {
struct Mailbox;
}

namespace IPC {
class SyncMessageFilter;
}

namespace content {
class CommandBufferProxyImpl;
class GpuChannelHost;
struct GpuRenderingStats;

struct GpuListenerInfo {
  GpuListenerInfo();
  ~GpuListenerInfo();

  base::WeakPtr<IPC::Listener> listener;
  scoped_refptr<base::MessageLoopProxy> loop;
};

class CONTENT_EXPORT GpuChannelHostFactory {
 public:
  typedef base::Callback<void(const gfx::Size)> CreateImageCallback;

  virtual ~GpuChannelHostFactory() {}

  virtual bool IsMainThread() = 0;
  virtual base::MessageLoop* GetMainLoop() = 0;
  virtual scoped_refptr<base::MessageLoopProxy> GetIOLoopProxy() = 0;
  virtual base::WaitableEvent* GetShutDownEvent() = 0;
  virtual scoped_ptr<base::SharedMemory> AllocateSharedMemory(size_t size) = 0;
  virtual int32 CreateViewCommandBuffer(
      int32 surface_id, const GPUCreateCommandBufferConfig& init_params) = 0;
  virtual void CreateImage(
      gfx::PluginWindowHandle window,
      int32 image_id,
      const CreateImageCallback& callback) = 0;
  virtual void DeleteImage(int32 image_id, int32 sync_point) = 0;
  virtual scoped_ptr<gfx::GpuMemoryBuffer> AllocateGpuMemoryBuffer(
      size_t width,
      size_t height,
      unsigned internalformat) = 0;
};

class GpuChannelHost : public IPC::Sender,
                       public base::RefCountedThreadSafe<GpuChannelHost> {
 public:
  
  static scoped_refptr<GpuChannelHost> Create(
      GpuChannelHostFactory* factory,
      int gpu_host_id,
      const gpu::GPUInfo& gpu_info,
      const IPC::ChannelHandle& channel_handle);

  
  
  static bool IsValidGpuMemoryBuffer(gfx::GpuMemoryBufferHandle handle);

  bool IsLost() const {
    DCHECK(channel_filter_.get());
    return channel_filter_->IsLost();
  }

  
  const gpu::GPUInfo& gpu_info() const { return gpu_info_; }

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  CommandBufferProxyImpl* CreateViewCommandBuffer(
      int32 surface_id,
      CommandBufferProxyImpl* share_group,
      const std::vector<int32>& attribs,
      const GURL& active_url,
      gfx::GpuPreference gpu_preference);

  
  CommandBufferProxyImpl* CreateOffscreenCommandBuffer(
      const gfx::Size& size,
      CommandBufferProxyImpl* share_group,
      const std::vector<int32>& attribs,
      const GURL& active_url,
      gfx::GpuPreference gpu_preference);

  
  scoped_ptr<media::VideoDecodeAccelerator> CreateVideoDecoder(
      int command_buffer_route_id,
      media::VideoCodecProfile profile,
      media::VideoDecodeAccelerator::Client* client);

  
  scoped_ptr<media::VideoEncodeAccelerator> CreateVideoEncoder(
      media::VideoEncodeAccelerator::Client* client);

  
  void DestroyCommandBuffer(CommandBufferProxyImpl* command_buffer);

  
  bool CollectRenderingStatsForSurface(
      int surface_id, GpuRenderingStats* stats);

  
  void AddRoute(int route_id, base::WeakPtr<IPC::Listener> listener);
  void RemoveRoute(int route_id);

  GpuChannelHostFactory* factory() const { return factory_; }
  int gpu_host_id() const { return gpu_host_id_; }

  
  
  
  base::SharedMemoryHandle ShareToGpuProcess(
      base::SharedMemoryHandle source_handle);

  
  
  
  
  bool GenerateMailboxNames(unsigned num, std::vector<gpu::Mailbox>* names);

  
  int32 ReserveTransferBufferId();

  
  
  
  gfx::GpuMemoryBufferHandle ShareGpuMemoryBufferToGpuProcess(
      gfx::GpuMemoryBufferHandle source_handle);

  
  int32 ReserveGpuMemoryBufferId();

 private:
  friend class base::RefCountedThreadSafe<GpuChannelHost>;
  GpuChannelHost(GpuChannelHostFactory* factory,
                 int gpu_host_id,
                 const gpu::GPUInfo& gpu_info);
  virtual ~GpuChannelHost();
  void Connect(const IPC::ChannelHandle& channel_handle);

  
  
  
  class MessageFilter : public IPC::ChannelProxy::MessageFilter {
   public:
    MessageFilter();

    
    void AddRoute(int route_id,
                  base::WeakPtr<IPC::Listener> listener,
                  scoped_refptr<base::MessageLoopProxy> loop);
    
    void RemoveRoute(int route_id);

    
    
    virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
    virtual void OnChannelError() OVERRIDE;

    

    
    bool IsLost() const;

    
    
    
    size_t GetMailboxNames(size_t num, std::vector<gpu::Mailbox>* names);

   private:
    virtual ~MessageFilter();
    bool OnControlMessageReceived(const IPC::Message& msg);

    
    void OnGenerateMailboxNamesReply(const std::vector<gpu::Mailbox>& names);

    
    
    typedef base::hash_map<int, GpuListenerInfo> ListenerMap;
    ListenerMap listeners_;

    
    mutable base::Lock lock_;

    
    bool lost_;

    
    std::vector<gpu::Mailbox> mailbox_name_pool_;

    
    size_t requested_mailboxes_;
  };

  
  
  
  
  
  GpuChannelHostFactory* const factory_;
  const int gpu_host_id_;

  const gpu::GPUInfo gpu_info_;

  scoped_ptr<IPC::SyncChannel> channel_;
  scoped_refptr<MessageFilter> channel_filter_;

  
  scoped_refptr<IPC::SyncMessageFilter> sync_filter_;

  
  base::AtomicSequenceNumber next_transfer_buffer_id_;

  
  base::AtomicSequenceNumber next_gpu_memory_buffer_id_;

  
  mutable base::Lock context_lock_;
  
  typedef base::hash_map<int, CommandBufferProxyImpl*> ProxyMap;
  ProxyMap proxies_;

  DISALLOW_COPY_AND_ASSIGN(GpuChannelHost);
};

}  

#endif  
