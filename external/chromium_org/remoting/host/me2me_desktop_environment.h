// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_ME2ME_DESKTOP_ENVIRONMENT_H_
#define REMOTING_HOST_ME2ME_DESKTOP_ENVIRONMENT_H_

#include "remoting/host/basic_desktop_environment.h"

namespace remoting {

class CurtainMode;
class HostWindow;
class LocalInputMonitor;

class Me2MeDesktopEnvironment : public BasicDesktopEnvironment {
 public:
  virtual ~Me2MeDesktopEnvironment();

  
  virtual scoped_ptr<ScreenControls> CreateScreenControls() OVERRIDE;
  virtual scoped_ptr<webrtc::ScreenCapturer> CreateVideoCapturer() OVERRIDE;
  virtual std::string GetCapabilities() const OVERRIDE;

 protected:
  friend class Me2MeDesktopEnvironmentFactory;
  Me2MeDesktopEnvironment(
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner);

  
  
  bool InitializeSecurity(
      base::WeakPtr<ClientSessionControl> client_session_control,
      bool curtain_enabled);

 private:
  
  
  scoped_ptr<CurtainMode> curtain_;

  
  scoped_ptr<HostWindow> disconnect_window_;

  
  scoped_ptr<LocalInputMonitor> local_input_monitor_;

  DISALLOW_COPY_AND_ASSIGN(Me2MeDesktopEnvironment);
};

class Me2MeDesktopEnvironmentFactory : public BasicDesktopEnvironmentFactory {
 public:
  Me2MeDesktopEnvironmentFactory(
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner);
  virtual ~Me2MeDesktopEnvironmentFactory();

  
  virtual scoped_ptr<DesktopEnvironment> Create(
      base::WeakPtr<ClientSessionControl> client_session_control) OVERRIDE;
  virtual void SetEnableCurtaining(bool enable) OVERRIDE;

 protected:
  bool curtain_enabled() const { return curtain_enabled_; }

 private:
  
  bool curtain_enabled_;

  DISALLOW_COPY_AND_ASSIGN(Me2MeDesktopEnvironmentFactory);
};

}  

#endif  
