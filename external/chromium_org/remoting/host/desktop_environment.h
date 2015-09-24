// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_ENVIRONMENT_H_
#define REMOTING_HOST_DESKTOP_ENVIRONMENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace webrtc {
class ScreenCapturer;
}  

namespace remoting {

class AudioCapturer;
class ClientSessionControl;
class InputInjector;
class ScreenControls;

class DesktopEnvironment {
 public:
  virtual ~DesktopEnvironment() {}

  
  
  virtual scoped_ptr<AudioCapturer> CreateAudioCapturer() = 0;
  virtual scoped_ptr<InputInjector> CreateInputInjector() = 0;
  virtual scoped_ptr<ScreenControls> CreateScreenControls() = 0;
  virtual scoped_ptr<webrtc::ScreenCapturer> CreateVideoCapturer() = 0;

  
  virtual std::string GetCapabilities() const = 0;

  
  
  virtual void SetCapabilities(const std::string& capabilities) = 0;
};

class DesktopEnvironmentFactory {
 public:
  virtual ~DesktopEnvironmentFactory() {}

  
  
  
  
  virtual scoped_ptr<DesktopEnvironment> Create(
      base::WeakPtr<ClientSessionControl> client_session_control) = 0;

  
  virtual void SetEnableCurtaining(bool enable) {}

  
  
  virtual bool SupportsAudioCapture() const = 0;
};

}  

#endif  
