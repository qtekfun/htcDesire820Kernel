// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_PROXY_MESSAGE_PIPE_ENDPOINT_H_
#define MOJO_SYSTEM_PROXY_MESSAGE_PIPE_ENDPOINT_H_

#include <stdint.h>

#include <deque>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "mojo/public/system/core.h"
#include "mojo/system/message_in_transit.h"
#include "mojo/system/message_pipe_endpoint.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class Channel;

class MOJO_SYSTEM_IMPL_EXPORT ProxyMessagePipeEndpoint
    : public MessagePipeEndpoint {
 public:
  ProxyMessagePipeEndpoint();
  virtual ~ProxyMessagePipeEndpoint();

  
  virtual void Close() OVERRIDE;
  virtual bool OnPeerClose() OVERRIDE;
  virtual MojoResult CanEnqueueMessage(
      const MessageInTransit* message,
      const std::vector<Dispatcher*>* dispatchers) OVERRIDE;
  virtual void EnqueueMessage(
      MessageInTransit* message,
      std::vector<scoped_refptr<Dispatcher> >* dispatchers) OVERRIDE;
  virtual void Attach(scoped_refptr<Channel> channel,
                      MessageInTransit::EndpointId local_id) OVERRIDE;
  virtual bool Run(MessageInTransit::EndpointId remote_id) OVERRIDE;

 private:
  bool is_attached() const {
    return !!channel_.get();
  }

  bool is_running() const {
    return remote_id_ != MessageInTransit::kInvalidEndpointId;
  }

#ifdef NDEBUG
  void AssertConsistentState() const {}
#else
  void AssertConsistentState() const;
#endif

  
  scoped_refptr<Channel> channel_;

  
  
  MessageInTransit::EndpointId local_id_;

  
  
  
  
  MessageInTransit::EndpointId remote_id_;

  bool is_open_;
  bool is_peer_open_;

  
  
  std::deque<MessageInTransit*> paused_message_queue_;

  DISALLOW_COPY_AND_ASSIGN(ProxyMessagePipeEndpoint);
};

}  
}  

#endif  
