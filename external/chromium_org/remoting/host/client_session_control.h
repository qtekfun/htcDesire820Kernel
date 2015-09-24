// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CLIENT_SESSION_CONTROL_H_
#define REMOTING_HOST_CLIENT_SESSION_CONTROL_H_

#include "base/basictypes.h"

namespace webrtc {
class DesktopVector;
}  

namespace remoting {

class ClientSessionControl {
 public:
  virtual ~ClientSessionControl() {}

  
  virtual const std::string& client_jid() const = 0;

  
  
  virtual void DisconnectSession() = 0;

  
  virtual void OnLocalMouseMoved(const webrtc::DesktopVector& position) = 0;

  
  virtual void SetDisableInputs(bool disable_inputs) = 0;
};

}  

#endif  
