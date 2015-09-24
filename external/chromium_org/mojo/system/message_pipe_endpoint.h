// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_MESSAGE_PIPE_ENDPOINT_H_
#define MOJO_SYSTEM_MESSAGE_PIPE_ENDPOINT_H_

#include <stdint.h>

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "mojo/public/system/core.h"
#include "mojo/system/message_in_transit.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class Channel;
class Dispatcher;
class Waiter;

class MOJO_SYSTEM_IMPL_EXPORT MessagePipeEndpoint {
 public:
  virtual ~MessagePipeEndpoint() {}

  
  virtual void Close() = 0;
  
  virtual bool OnPeerClose() = 0;
  
  
  
  
  virtual MojoResult CanEnqueueMessage(
      const MessageInTransit* message,
      const std::vector<Dispatcher*>* dispatchers) = 0;
  
  
  
  
  virtual void EnqueueMessage(
      MessageInTransit* message,
      std::vector<scoped_refptr<Dispatcher> >* dispatchers) = 0;

  
  
  
  
  
  
  
  
  virtual void CancelAllWaiters();
  virtual MojoResult ReadMessage(
      void* bytes, uint32_t* num_bytes,
      std::vector<scoped_refptr<Dispatcher> >* dispatchers,
      uint32_t* num_dispatchers,
      MojoReadMessageFlags flags);
  virtual MojoResult AddWaiter(Waiter* waiter,
                               MojoWaitFlags flags,
                               MojoResult wake_result);
  virtual void RemoveWaiter(Waiter* waiter);

  
  
  
  virtual void Attach(scoped_refptr<Channel> channel,
                      MessageInTransit::EndpointId local_id);
  
  virtual bool Run(MessageInTransit::EndpointId remote_id);

 protected:
  MessagePipeEndpoint() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MessagePipeEndpoint);
};

}  
}  

#endif  
