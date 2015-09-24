// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_FRAME_OBSERVER_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_FRAME_OBSERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"

namespace content {

class RendererPpapiHost;
class RenderFrame;
class RenderFrameImpl;

class CONTENT_EXPORT RenderFrameObserver : public IPC::Listener,
                                           public IPC::Sender {
 public:
  
  
  virtual void OnDestruct();

  
  virtual void DidCreatePepperPlugin(RendererPpapiHost* host) {}

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  RenderFrame* render_frame() const;
  int routing_id() const { return routing_id_; }

 protected:
  explicit RenderFrameObserver(RenderFrame* render_frame);
  virtual ~RenderFrameObserver();

 private:
  friend class RenderFrameImpl;

  
  
  void RenderFrameGone();

  RenderFrame* render_frame_;
  
  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(RenderFrameObserver);
};

}  

#endif  
