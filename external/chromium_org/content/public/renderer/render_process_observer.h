// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_PROCESS_OBSERVER_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_PROCESS_OBSERVER_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace IPC {
class Message;
}

namespace content {

class CONTENT_EXPORT RenderProcessObserver {
 public:
  RenderProcessObserver() {}
  virtual ~RenderProcessObserver() {}

  
  virtual bool OnControlMessageReceived(const IPC::Message& message);

  
  virtual void OnRenderProcessShutdown() {}

  
  virtual void WebKitInitialized() {}

  
  virtual void PluginListChanged() {}

  virtual void IdleNotification() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(RenderProcessObserver);
};

}  

#endif  
