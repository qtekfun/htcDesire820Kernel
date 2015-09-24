// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_HOST_EVENT_DISPATCHER_H_
#define REMOTING_PROTOCOL_HOST_EVENT_DISPATCHER_H_

#include "remoting/protocol/channel_dispatcher_base.h"
#include "remoting/protocol/message_reader.h"

namespace remoting {
namespace protocol {

class EventMessage;
class InputStub;

class HostEventDispatcher : public ChannelDispatcherBase {
 public:
  typedef base::Callback<void(int64)> SequenceNumberCallback;

  HostEventDispatcher();
  virtual ~HostEventDispatcher();

  
  
  
  void set_input_stub(InputStub* input_stub) { input_stub_ = input_stub; }

  
  
  void set_sequence_number_callback(const SequenceNumberCallback& value) {
    sequence_number_callback_ = value;
  }

 protected:
  
  virtual void OnInitialized() OVERRIDE;

 private:
  void OnMessageReceived(scoped_ptr<EventMessage> message,
                         const base::Closure& done_task);

  InputStub* input_stub_;
  SequenceNumberCallback sequence_number_callback_;

  ProtobufMessageReader<EventMessage> reader_;

  DISALLOW_COPY_AND_ASSIGN(HostEventDispatcher);
};

}  
}  

#endif  
