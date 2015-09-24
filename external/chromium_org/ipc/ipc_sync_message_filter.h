// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_SYNC_MESSAGE_FILTER_H_
#define IPC_IPC_SYNC_MESSAGE_FILTER_H_

#include <set>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sync_message.h"

namespace base {
class MessageLoopProxy;
class WaitableEvent;
}

namespace IPC {

class IPC_EXPORT SyncMessageFilter : public ChannelProxy::MessageFilter,
                                     public Sender {
 public:
  explicit SyncMessageFilter(base::WaitableEvent* shutdown_event);

  
  virtual bool Send(Message* message) OVERRIDE;

  
  virtual void OnFilterAdded(Channel* channel) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;
  virtual bool OnMessageReceived(const Message& message) OVERRIDE;

 protected:
  virtual ~SyncMessageFilter();

 private:
  void SendOnIOThread(Message* message);
  
  void SignalAllEvents();

  
  Channel* channel_;

  
  scoped_refptr<base::MessageLoopProxy> listener_loop_;

  
  scoped_refptr<base::MessageLoopProxy> io_loop_;

  typedef std::set<PendingSyncMsg*> PendingSyncMessages;
  PendingSyncMessages pending_sync_messages_;

  
  base::Lock lock_;

  base::WaitableEvent* shutdown_event_;

  DISALLOW_COPY_AND_ASSIGN(SyncMessageFilter);
};

}  

#endif  
