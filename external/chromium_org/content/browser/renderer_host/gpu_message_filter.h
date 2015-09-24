// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_GPU_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_GPU_MESSAGE_FILTER_H_

#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/common/gpu/gpu_process_launch_causes.h"
#include "content/public/browser/browser_message_filter.h"
#include "ui/gfx/native_widget_types.h"

class GpuProcessHost;
struct GPUCreateCommandBufferConfig;

namespace gpu {
struct GPUInfo;
}

namespace content {
class RenderWidgetHelper;
class RenderWidgetHostViewFrameSubscriber;

class GpuMessageFilter : public BrowserMessageFilter {
 public:
  GpuMessageFilter(int render_process_id,
                   RenderWidgetHelper* render_widget_helper);

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  
  
  void SurfaceUpdated(int32 surface_id);

  
  
  void BeginFrameSubscription(
      int route_id,
      scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber);
  void EndFrameSubscription(int route_id);

 private:
  friend class BrowserThread;
  friend class base::DeleteHelper<GpuMessageFilter>;
  struct CreateViewCommandBufferRequest;
  struct FrameSubscription;

  virtual ~GpuMessageFilter();

  
  void OnEstablishGpuChannel(CauseForGpuLaunch,
                             IPC::Message* reply);
  void OnCreateViewCommandBuffer(
      int32 surface_id,
      const GPUCreateCommandBufferConfig& init_params,
      IPC::Message* reply);
  
  void EstablishChannelCallback(scoped_ptr<IPC::Message> reply,
                                const IPC::ChannelHandle& channel,
                                const gpu::GPUInfo& gpu_info);
  void CreateCommandBufferCallback(scoped_ptr<IPC::Message> reply,
                                   int32 route_id);

  void BeginAllFrameSubscriptions();
  void EndAllFrameSubscriptions();
  void BeginFrameSubscriptionInternal(
      linked_ptr<FrameSubscription> subscription);
  void EndFrameSubscriptionInternal(
      linked_ptr<FrameSubscription> subscription);

  int gpu_process_id_;
  int render_process_id_;
  bool share_contexts_;

  scoped_refptr<RenderWidgetHelper> render_widget_helper_;
  std::vector<linked_ptr<CreateViewCommandBufferRequest> > pending_requests_;

  base::WeakPtrFactory<GpuMessageFilter> weak_ptr_factory_;

  typedef std::vector<linked_ptr<FrameSubscription> > FrameSubscriptionList;
  FrameSubscriptionList frame_subscription_list_;

  DISALLOW_COPY_AND_ASSIGN(GpuMessageFilter);
};

}  

#endif  
