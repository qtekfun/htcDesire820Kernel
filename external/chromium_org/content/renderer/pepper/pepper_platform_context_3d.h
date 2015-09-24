// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_PLATFORM_CONTEXT_3D_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_PLATFORM_CONTEXT_3D_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/common/gpu/client/webgraphicscontext3d_command_buffer_impl.h"
#include "gpu/command_buffer/common/mailbox.h"

class CommandBufferProxy;
namespace gpu {
class CommandBuffer;
struct Mailbox;
}  

namespace content {
class ContextProviderCommandBuffer;
class GpuChannelHost;

class PlatformContext3D {
 public:
  explicit PlatformContext3D();
  ~PlatformContext3D();

  
  bool Init(const int32* attrib_list, PlatformContext3D* share_context);

  
  void GetBackingMailbox(gpu::Mailbox* mailbox, uint32* sync_point);

  
  
  void InsertSyncPointForBackingMailbox();

  
  bool IsOpaque();

  
  
  
  gpu::CommandBuffer* GetCommandBuffer();

  
  gpu::GpuControl* GetGpuControl();

  
  
  int GetCommandBufferRouteId();

  
  
  typedef base::Callback<void(const std::string&, int)>
        ConsoleMessageCallback;
  void SetContextLostCallback(const base::Closure& callback);

  
  
  void SetOnConsoleMessageCallback(const ConsoleMessageCallback& callback);

  
  void Echo(const base::Closure& task);

 private:
  bool InitRaw();
  void OnContextLost();
  void OnConsoleMessage(const std::string& msg, int id);

  scoped_refptr<GpuChannelHost> channel_;
  gpu::Mailbox mailbox_;
  uint32 sync_point_;
  bool has_alpha_;
  CommandBufferProxyImpl* command_buffer_;
  base::Closure context_lost_callback_;
  ConsoleMessageCallback console_message_callback_;
  base::WeakPtrFactory<PlatformContext3D> weak_ptr_factory_;
};

}  

#endif  
