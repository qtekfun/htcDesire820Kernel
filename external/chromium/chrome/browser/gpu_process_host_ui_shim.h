// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GPU_PROCESS_HOST_UI_SHIM_H_
#define CHROME_BROWSER_GPU_PROCESS_HOST_UI_SHIM_H_
#pragma once


#include <queue>

#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/gpu/gpu_channel_manager.h"
#include "content/common/gpu/gpu_info.h"
#include "content/common/gpu_feature_flags.h"
#include "content/common/gpu_process_launch_causes.h"
#include "content/common/message_router.h"

namespace gfx {
class Size;
}

struct GPUCreateCommandBufferConfig;
struct GpuHostMsg_AcceleratedSurfaceSetIOSurface_Params;
struct GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params;

namespace IPC {
struct ChannelHandle;
class Message;
}

class RouteToGpuProcessHostUIShimTask : public Task {
 public:
  RouteToGpuProcessHostUIShimTask(int host_id, const IPC::Message& msg);
  ~RouteToGpuProcessHostUIShimTask();

 private:
  virtual void Run();

  int host_id_;
  IPC::Message msg_;
};

class GpuProcessHostUIShim
    : public IPC::Channel::Listener,
      public IPC::Channel::Sender,
      public base::NonThreadSafe {
 public:
  
  
  static GpuProcessHostUIShim* Create(int host_id);

  
  
  
  static void Destroy(int host_id);

  
  static void DestroyAll();

  static GpuProcessHostUIShim* FromID(int host_id);

  
  virtual bool Send(IPC::Message* msg);

  
  
  
  
  virtual bool OnMessageReceived(const IPC::Message& message);

#if defined(OS_MACOSX)
  
  void DidDestroyAcceleratedSurface(int renderer_id, int32 render_view_id);

  
  
  static void SendToGpuHost(int host_id, IPC::Message* msg);
#endif

 private:
  explicit GpuProcessHostUIShim(int host_id);
  virtual ~GpuProcessHostUIShim();

  
  bool OnControlMessageReceived(const IPC::Message& message);

  void OnLogMessage(int level, const std::string& header,
      const std::string& message);
#if defined(OS_LINUX) && !defined(TOUCH_UI) || defined(OS_WIN)
  void OnResizeView(int32 renderer_id,
                    int32 render_view_id,
                    int32 command_buffer_route_id,
                    gfx::Size size);
#elif defined(OS_MACOSX)
  void OnAcceleratedSurfaceSetIOSurface(
      const GpuHostMsg_AcceleratedSurfaceSetIOSurface_Params& params);
  void OnAcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params);
#endif
#if defined(OS_WIN)
  void OnScheduleComposite(int32 renderer_id, int32 render_view_id);
#endif

  
  int host_id_;

  
  
  
  GpuChannelManager* gpu_channel_manager_;

  
  
  IPC::Channel::Sender* ui_thread_sender_;
};

#endif  

