// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_CHANNEL_H_
#define MOJO_SYSTEM_CHANNEL_H_

#include <stdint.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "mojo/public/system/core.h"
#include "mojo/system/message_in_transit.h"
#include "mojo/system/message_pipe.h"
#include "mojo/system/raw_channel.h"
#include "mojo/system/system_impl_export.h"

namespace base {
class MessageLoop;
}

namespace mojo {
namespace system {

class MOJO_SYSTEM_IMPL_EXPORT Channel
    : public base::RefCountedThreadSafe<Channel>,
      public RawChannel::Delegate {
 public:
  
  static const MessageInTransit::EndpointId kBootstrapEndpointId = 1;

  Channel();

  
  
  
  
  bool Init(const PlatformChannelHandle& handle);

  
  
  void Shutdown();

  
  
  
  
  
  
  MessageInTransit::EndpointId AttachMessagePipeEndpoint(
      scoped_refptr<MessagePipe> message_pipe, unsigned port);
  void RunMessagePipeEndpoint(MessageInTransit::EndpointId local_id,
                              MessageInTransit::EndpointId remote_id);

  
  bool WriteMessage(MessageInTransit* message);

  
  
  
  void DetachMessagePipeEndpoint(MessageInTransit::EndpointId local_id);

 private:
  friend class base::RefCountedThreadSafe<Channel>;
  virtual ~Channel();

  
  virtual void OnReadMessage(const MessageInTransit& message) OVERRIDE;
  virtual void OnFatalError(FatalError fatal_error) OVERRIDE;

  
  void OnReadMessageForDownstream(const MessageInTransit& message);
  void OnReadMessageForChannel(const MessageInTransit& message);

  
  void HandleRemoteError(const base::StringPiece& error_message);
  
  void HandleLocalError(const base::StringPiece& error_message);

  struct EndpointInfo {
    EndpointInfo();
    EndpointInfo(scoped_refptr<MessagePipe> message_pipe, unsigned port);
    ~EndpointInfo();

    scoped_refptr<MessagePipe> message_pipe;
    unsigned port;
  };

  base::ThreadChecker creation_thread_checker_;

  
  
  
  
  
  base::Lock lock_;  

  scoped_ptr<RawChannel> raw_channel_;

  typedef base::hash_map<MessageInTransit::EndpointId, EndpointInfo>
      IdToEndpointInfoMap;
  IdToEndpointInfoMap local_id_to_endpoint_info_map_;
  
  
  MessageInTransit::EndpointId next_local_id_;

  DISALLOW_COPY_AND_ASSIGN(Channel);
};

}  
}  

#endif  
