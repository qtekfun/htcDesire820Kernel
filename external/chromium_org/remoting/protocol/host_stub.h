// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_HOST_STUB_H_
#define REMOTING_PROTOCOL_HOST_STUB_H_

#include "base/basictypes.h"

namespace remoting {
namespace protocol {

class AudioControl;
class Capabilities;
class ClientResolution;
class ExtensionMessage;
class PairingResponse;
class PairingRequest;
class VideoControl;

class HostStub {
 public:
  HostStub() {}

  
  
  virtual void NotifyClientResolution(const ClientResolution& resolution) = 0;

  
  
  virtual void ControlVideo(const VideoControl& video_control) = 0;

  
  
  virtual void ControlAudio(const AudioControl& audio_control) = 0;

  
  virtual void SetCapabilities(const Capabilities& capabilities) = 0;

  
  virtual void RequestPairing(const PairingRequest& pairing_request) = 0;

  
  virtual void DeliverClientMessage(const ExtensionMessage& message) = 0;

 protected:
  virtual ~HostStub() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(HostStub);
};

}  
}  

#endif  
