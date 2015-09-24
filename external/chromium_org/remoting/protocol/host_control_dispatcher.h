// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_HOST_CONTROL_DISPATCHER_H_
#define REMOTING_PROTOCOL_HOST_CONTROL_DISPATCHER_H_

#include "remoting/protocol/buffered_socket_writer.h"
#include "remoting/protocol/channel_dispatcher_base.h"
#include "remoting/protocol/client_stub.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/cursor_shape_stub.h"
#include "remoting/protocol/message_reader.h"

namespace net {
class StreamSocket;
}  

namespace remoting {
namespace protocol {

class ControlMessage;
class HostStub;
class PairingResponse;
class Session;

class HostControlDispatcher : public ChannelDispatcherBase,
                              public ClientStub {
 public:
  HostControlDispatcher();
  virtual ~HostControlDispatcher();

  
  virtual void SetCapabilities(const Capabilities& capabilities) OVERRIDE;
  virtual void SetPairingResponse(
      const PairingResponse& pairing_response) OVERRIDE;
  virtual void DeliverHostMessage(
      const ExtensionMessage& message) OVERRIDE;

  
  virtual void InjectClipboardEvent(const ClipboardEvent& event) OVERRIDE;

  
  virtual void SetCursorShape(const CursorShapeInfo& cursor_shape) OVERRIDE;

  
  
  void set_clipboard_stub(ClipboardStub* clipboard_stub) {
    clipboard_stub_ = clipboard_stub;
  }

  
  
  void set_host_stub(HostStub* host_stub) { host_stub_ = host_stub; }

 protected:
  
  virtual void OnInitialized() OVERRIDE;

 private:
  void OnMessageReceived(scoped_ptr<ControlMessage> message,
                         const base::Closure& done_task);

  ClipboardStub* clipboard_stub_;
  HostStub* host_stub_;

  ProtobufMessageReader<ControlMessage> reader_;
  BufferedSocketWriter writer_;

  DISALLOW_COPY_AND_ASSIGN(HostControlDispatcher);
};

}  
}  

#endif  
