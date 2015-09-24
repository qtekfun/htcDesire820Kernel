// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_CLIENT_STUB_H_
#define REMOTING_PROTOCOL_CLIENT_STUB_H_

#include "base/basictypes.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/cursor_shape_stub.h"

namespace remoting {
namespace protocol {

class Capabilities;
class ExtensionMessage;
class PairingResponse;

class ClientStub : public ClipboardStub,
                   public CursorShapeStub {
 public:
  ClientStub() {}
  virtual ~ClientStub() {}

  
  virtual void SetCapabilities(const Capabilities& capabilities) = 0;

  
  virtual void SetPairingResponse(const PairingResponse& pairing_response) = 0;

  
  virtual void DeliverHostMessage(const ExtensionMessage& message) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ClientStub);
};

}  
}  

#endif  
