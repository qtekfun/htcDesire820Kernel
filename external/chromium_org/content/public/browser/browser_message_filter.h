// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_MESSAGE_FILTER_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_MESSAGE_FILTER_H_

#include "base/memory/ref_counted.h"
#include "base/process/process.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_thread.h"
#include "ipc/ipc_channel_proxy.h"

#if defined(OS_WIN)
#include "base/synchronization/lock.h"
#endif

namespace base {
class TaskRunner;
}

namespace content {
struct BrowserMessageFilterTraits;

class CONTENT_EXPORT BrowserMessageFilter
    : public base::RefCountedThreadSafe<
          BrowserMessageFilter, BrowserMessageFilterTraits>,
      public IPC::Sender {
 public:
  BrowserMessageFilter();

  
  
  virtual void OnFilterAdded(IPC::Channel* channel) {}
  virtual void OnFilterRemoved() {}
  virtual void OnChannelClosing() {}
  virtual void OnChannelConnected(int32 peer_pid) {}

  
  
  
  virtual void OnDestruct() const;

  
  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  
  

  
  
  virtual void OverrideThreadForMessage(
      const IPC::Message& message,
      BrowserThread::ID* thread) {}

  
  
  
  
  virtual base::TaskRunner* OverrideTaskRunnerForMessage(
      const IPC::Message& message);

  
  
  
  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) = 0;

  
  base::ProcessHandle PeerHandle();

  
  base::ProcessId peer_pid() const { return peer_pid_; }

  void set_peer_pid_for_testing(base::ProcessId peer_pid) {
    peer_pid_ = peer_pid;
  }

  
  
  static bool CheckCanDispatchOnUI(const IPC::Message& message,
                                   IPC::Sender* sender);

  
  
  virtual void BadMessageReceived();

 protected:
  virtual ~BrowserMessageFilter();

 private:
  friend class base::RefCountedThreadSafe<BrowserMessageFilter,
                                          BrowserMessageFilterTraits>;

  class Internal;
  friend class BrowserChildProcessHostImpl;
  friend class BrowserPpapiHost;
  friend class RenderProcessHostImpl;

  
  
  
  IPC::ChannelProxy::MessageFilter* GetFilter();

  
  
  
  
  Internal* internal_;

  IPC::Channel* channel_;
  base::ProcessId peer_pid_;

#if defined(OS_WIN)
  base::Lock peer_handle_lock_;
  base::ProcessHandle peer_handle_;
#endif
};

struct BrowserMessageFilterTraits {
  static void Destruct(const BrowserMessageFilter* filter) {
    filter->OnDestruct();
  }
};

}  

#endif  
