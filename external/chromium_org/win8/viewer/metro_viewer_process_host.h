// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_VIEWER_METRO_VIEWER_PROCESS_HOST_H_
#define WIN8_VIEWER_METRO_VIEWER_PROCESS_HOST_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/threading/non_thread_safe.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "ui/gfx/native_widget_types.h"

namespace base {
class SingleThreadTaskRunner;
class WaitableEvent;
}

namespace IPC {
class Message;
}

namespace win8 {

class MetroViewerProcessHost : public IPC::Listener,
                               public IPC::Sender,
                               public base::NonThreadSafe {
 public:
  
  
  
  
  
  
  explicit MetroViewerProcessHost(
      base::SingleThreadTaskRunner* ipc_task_runner);
  virtual ~MetroViewerProcessHost();

  
  
  base::ProcessId GetViewerProcessId();

  
  
  
  
  
  bool LaunchViewerAndWaitForConnection(
      const base::string16& app_user_model_id);

 private:
  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE = 0;

  
  
  virtual void OnSetTargetSurface(gfx::NativeViewId target_surface) = 0;

  
  
  virtual void OnOpenURL(const string16& url) = 0;

  
  
  
  virtual void OnHandleSearchRequest(const string16& search_string) = 0;

  
  virtual void OnWindowSizeChanged(uint32 width, uint32 height) = 0;

  void NotifyChannelConnected();

  
  
  
  
  class InternalMessageFilter : public IPC::ChannelProxy::MessageFilter {
   public:
    InternalMessageFilter(MetroViewerProcessHost* owner);

    
    virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;

   private:
    MetroViewerProcessHost* owner_;
    DISALLOW_COPY_AND_ASSIGN(InternalMessageFilter);
  };

  scoped_ptr<IPC::ChannelProxy> channel_;
  scoped_ptr<base::WaitableEvent> channel_connected_event_;

  DISALLOW_COPY_AND_ASSIGN(MetroViewerProcessHost);
};

}  

#endif  
