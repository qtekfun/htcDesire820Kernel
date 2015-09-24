// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_MESSAGE_PIPE_H_
#define MOJO_SYSTEM_MESSAGE_PIPE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "mojo/public/system/core.h"
#include "mojo/system/message_in_transit.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class Channel;
class Dispatcher;
class MessagePipeEndpoint;
class Waiter;

class MOJO_SYSTEM_IMPL_EXPORT MessagePipe :
    public base::RefCountedThreadSafe<MessagePipe> {
 public:
  MessagePipe(scoped_ptr<MessagePipeEndpoint> endpoint_0,
              scoped_ptr<MessagePipeEndpoint> endpoint_1);

  
  
  MessagePipe();

  
  static unsigned GetPeerPort(unsigned port);

  
  
  void CancelAllWaiters(unsigned port);
  void Close(unsigned port);
  
  
  MojoResult WriteMessage(unsigned port,
                          const void* bytes, uint32_t num_bytes,
                          const std::vector<Dispatcher*>* dispatchers,
                          MojoWriteMessageFlags flags);
  
  
  MojoResult ReadMessage(unsigned port,
                         void* bytes, uint32_t* num_bytes,
                         std::vector<scoped_refptr<Dispatcher> >* dispatchers,
                         uint32_t* num_dispatchers,
                         MojoReadMessageFlags flags);
  MojoResult AddWaiter(unsigned port,
                       Waiter* waiter,
                       MojoWaitFlags flags,
                       MojoResult wake_result);
  void RemoveWaiter(unsigned port, Waiter* waiter);

  
  
  
  
  MojoResult EnqueueMessage(unsigned port,
                            MessageInTransit* message,
                            const std::vector<Dispatcher*>* dispatchers);

  
  void Attach(unsigned port,
              scoped_refptr<Channel> channel,
              MessageInTransit::EndpointId local_id);
  void Run(unsigned port, MessageInTransit::EndpointId remote_id);

 private:
  friend class base::RefCountedThreadSafe<MessagePipe>;
  virtual ~MessagePipe();

  
  
  MojoResult HandleControlMessage(unsigned port, MessageInTransit* message);

  base::Lock lock_;  
  scoped_ptr<MessagePipeEndpoint> endpoints_[2];

  DISALLOW_COPY_AND_ASSIGN(MessagePipe);
};

}  
}  

#endif  
