// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_FORWARDING_MESSAGE_FILTER_H_
#define IPC_IPC_FORWARDING_MESSAGE_FILTER_H_

#include <map>
#include <set>

#include "base/bind.h"
#include "base/callback_forward.h"
#include "base/synchronization/lock.h"
#include "base/task_runner.h"
#include "ipc/ipc_channel_proxy.h"

namespace IPC {

class IPC_EXPORT ForwardingMessageFilter : public ChannelProxy::MessageFilter {
 public:

  
  
  typedef base::Callback<void(const Message&)> Handler;

  
  
  
  
  
  ForwardingMessageFilter(
      const uint32* message_ids_to_filter,
      size_t num_message_ids_to_filter,
      base::TaskRunner* target_task_runner);

  
  void AddRoute(int routing_id, const Handler& handler);
  void RemoveRoute(int routing_id);

  
  virtual bool OnMessageReceived(const Message& message) OVERRIDE;

 private:
  friend class ChannelProxy::MessageFilter;
  virtual ~ForwardingMessageFilter();

  std::set<int> message_ids_to_filter_;

  
  
  scoped_refptr<base::TaskRunner> target_task_runner_;

  
  base::Lock handlers_lock_;

  
  std::map<int, Handler> handlers_;

  DISALLOW_COPY_AND_ASSIGN(ForwardingMessageFilter);
};

}  

#endif  
