// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_HUNG_PLUGIN_FILTER_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_HUNG_PLUGIN_FILTER_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/lock.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sync_message_filter.h"
#include "ppapi/proxy/host_dispatcher.h"

namespace content {

class PepperHungPluginFilter
    : public ppapi::proxy::HostDispatcher::SyncMessageStatusReceiver {
 public:
  
  
  
  
  PepperHungPluginFilter(const base::FilePath& plugin_path,
                         int frame_routing_id,
                         int plugin_child_id);

  
  virtual void BeginBlockOnSyncMessage() OVERRIDE;
  virtual void EndBlockOnSyncMessage() OVERRIDE;

  
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelError() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 protected:
  virtual ~PepperHungPluginFilter();

 private:
  
  void EnsureTimerScheduled();

  
  
  void MayHaveBecomeUnhung();

  
  base::TimeTicks GetHungTime() const;

  
  
  bool IsHung() const;

  
  void OnHangTimer();

  
  void SendHungMessage(bool is_hung);

  base::Lock lock_;

  base::FilePath plugin_path_;
  int frame_routing_id_;
  int plugin_child_id_;

  
  
  
  scoped_refptr<IPC::SyncMessageFilter> filter_;

  scoped_refptr<base::MessageLoopProxy> io_loop_;

  
  
  
  
  base::TimeTicks began_blocking_time_;

  
  
  
  base::TimeTicks last_message_received_;

  
  int pending_sync_message_count_;

  
  
  
  bool hung_plugin_showing_;

  bool timer_task_pending_;

  DISALLOW_COPY_AND_ASSIGN(PepperHungPluginFilter);
};

}  

#endif  
