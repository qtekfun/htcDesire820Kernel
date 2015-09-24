// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_CLIENT_USER_INTERFACE_H_
#define REMOTING_CLIENT_CLIENT_USER_INTERFACE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/connection_to_host.h"
#include "remoting/protocol/third_party_client_authenticator.h"

namespace remoting {

namespace protocol {
class ClipboardStub;
class CursorShapeStub;
class PairingResponse;
}  

class ClientUserInterface {
 public:
  virtual ~ClientUserInterface() {}

  
  virtual void OnConnectionState(protocol::ConnectionToHost::State state,
                                 protocol::ErrorCode error) = 0;
  virtual void OnConnectionReady(bool ready) = 0;

  
  
  virtual void SetCapabilities(const std::string& capabilities) = 0;

  
  virtual void SetPairingResponse(
      const protocol::PairingResponse& pairing_response) = 0;

  
  virtual void DeliverHostMessage(
      const protocol::ExtensionMessage& message) = 0;

  
  virtual protocol::ClipboardStub* GetClipboardStub() = 0;

  
  virtual protocol::CursorShapeStub* GetCursorShapeStub() = 0;

  
  
  virtual scoped_ptr<protocol::ThirdPartyClientAuthenticator::TokenFetcher>
  GetTokenFetcher(const std::string& host_public_key) = 0;
};

}  

#endif  
