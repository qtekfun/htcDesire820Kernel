// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_SYNC_CHANNEL_H_
#define IPC_IPC_SYNC_CHANNEL_H_

#include <string>
#include <deque>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event_watcher.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sync_message.h"

namespace base {
class WaitableEvent;
};

namespace IPC {

class SyncMessage;

class IPC_EXPORT SyncChannel : public ChannelProxy {
 public:
  enum RestrictDispatchGroup {
    kRestrictDispatchGroup_None = 0,
  };

  
  
  SyncChannel(const IPC::ChannelHandle& channel_handle,
              Channel::Mode mode,
              Listener* listener,
              base::SingleThreadTaskRunner* ipc_task_runner,
              bool create_pipe_now,
              base::WaitableEvent* shutdown_event);

  
  
  
  SyncChannel(Listener* listener,
              base::SingleThreadTaskRunner* ipc_task_runner,
              base::WaitableEvent* shutdown_event);

  virtual ~SyncChannel();

  virtual bool Send(Message* message) OVERRIDE;
  virtual bool SendWithTimeout(Message* message, int timeout_ms);

  
  void set_sync_messages_with_no_timeout_allowed(bool value) {
    sync_messages_with_no_timeout_allowed_ = value;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetRestrictDispatchChannelGroup(int group);

 protected:
  class ReceivedSyncMsgQueue;
  friend class ReceivedSyncMsgQueue;

  
  
  
  class SyncContext : public Context {
   public:
    SyncContext(Listener* listener,
                base::SingleThreadTaskRunner* ipc_task_runner,
                base::WaitableEvent* shutdown_event);

    
    
    void Push(SyncMessage* sync_msg);

    
    
    bool Pop();

    
    
    base::WaitableEvent* GetSendDoneEvent();

    
    
    base::WaitableEvent* GetDispatchEvent();

    void DispatchMessages();

    
    
    
    bool TryToUnblockListener(const Message* msg);

    
    
    void OnSendTimeout(int message_id);

    base::WaitableEvent* shutdown_event() { return shutdown_event_; }

    ReceivedSyncMsgQueue* received_sync_msgs() {
      return received_sync_msgs_.get();
    }

    void set_restrict_dispatch_group(int group) {
      restrict_dispatch_group_ = group;
    }

    int restrict_dispatch_group() const {
      return restrict_dispatch_group_;
    }

    base::WaitableEventWatcher::EventCallback MakeWaitableEventCallback();

   private:
    virtual ~SyncContext();
    

    
    virtual void Clear() OVERRIDE;

    
    virtual bool OnMessageReceived(const Message& msg) OVERRIDE;
    virtual void OnChannelError() OVERRIDE;
    virtual void OnChannelOpened() OVERRIDE;
    virtual void OnChannelClosed() OVERRIDE;

    
    void CancelPendingSends();

    void OnWaitableEventSignaled(base::WaitableEvent* event);

    typedef std::deque<PendingSyncMsg> PendingSyncMessageQueue;
    PendingSyncMessageQueue deserializers_;
    base::Lock deserializers_lock_;

    scoped_refptr<ReceivedSyncMsgQueue> received_sync_msgs_;

    base::WaitableEvent* shutdown_event_;
    base::WaitableEventWatcher shutdown_watcher_;
    base::WaitableEventWatcher::EventCallback shutdown_watcher_callback_;
    int restrict_dispatch_group_;
  };

 private:
  void OnWaitableEventSignaled(base::WaitableEvent* arg);

  SyncContext* sync_context() {
    return reinterpret_cast<SyncContext*>(context());
  }

  
  
  static void WaitForReply(
      SyncContext* context, base::WaitableEvent* pump_messages_event);

  
  
  static void WaitForReplyWithNestedMessageLoop(SyncContext* context);

  
  void StartWatching();

  bool sync_messages_with_no_timeout_allowed_;

  
  base::WaitableEventWatcher dispatch_watcher_;
  base::WaitableEventWatcher::EventCallback dispatch_watcher_callback_;

  DISALLOW_COPY_AND_ASSIGN(SyncChannel);
};

}  

#endif  
