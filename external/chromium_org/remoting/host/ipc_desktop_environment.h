// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IPC_DESKTOP_ENVIRONMENT_H_
#define REMOTING_HOST_IPC_DESKTOP_ENVIRONMENT_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/host/desktop_environment.h"
#include "remoting/host/desktop_session_connector.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace IPC {
class Sender;
}  

namespace remoting {

class ClientSessionControl;
class DesktopSessionProxy;
class ScreenResolution;

class IpcDesktopEnvironment : public DesktopEnvironment {
 public:
  
  
  
  IpcDesktopEnvironment(
      scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> capture_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
      base::WeakPtr<ClientSessionControl> client_session_control,
      base::WeakPtr<DesktopSessionConnector> desktop_session_connector,
      bool virtual_terminal);
  virtual ~IpcDesktopEnvironment();

  
  virtual scoped_ptr<AudioCapturer> CreateAudioCapturer() OVERRIDE;
  virtual scoped_ptr<InputInjector> CreateInputInjector() OVERRIDE;
  virtual scoped_ptr<ScreenControls> CreateScreenControls() OVERRIDE;
  virtual scoped_ptr<webrtc::ScreenCapturer> CreateVideoCapturer() OVERRIDE;
  virtual std::string GetCapabilities() const OVERRIDE;
  virtual void SetCapabilities(const std::string& capabilities) OVERRIDE;

 private:
  scoped_refptr<DesktopSessionProxy> desktop_session_proxy_;

  DISALLOW_COPY_AND_ASSIGN(IpcDesktopEnvironment);
};

class IpcDesktopEnvironmentFactory
    : public DesktopEnvironmentFactory,
      public DesktopSessionConnector {
 public:
  
  
  IpcDesktopEnvironmentFactory(
      scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> capture_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
      IPC::Sender* daemon_channel);
  virtual ~IpcDesktopEnvironmentFactory();

  
  virtual scoped_ptr<DesktopEnvironment> Create(
      base::WeakPtr<ClientSessionControl> client_session_control) OVERRIDE;
  virtual void SetEnableCurtaining(bool enable) OVERRIDE;
  virtual bool SupportsAudioCapture() const OVERRIDE;

  
  virtual void ConnectTerminal(
      DesktopSessionProxy* desktop_session_proxy,
      const ScreenResolution& resolution,
      bool virtual_terminal) OVERRIDE;
  virtual void DisconnectTerminal(
      DesktopSessionProxy* desktop_session_proxy) OVERRIDE;
  virtual void SetScreenResolution(
      DesktopSessionProxy* desktop_session_proxy,
      const ScreenResolution& resolution) OVERRIDE;
  virtual void OnDesktopSessionAgentAttached(
      int terminal_id,
      base::ProcessHandle desktop_process,
      IPC::PlatformFileForTransit desktop_pipe) OVERRIDE;
  virtual void OnTerminalDisconnected(int terminal_id) OVERRIDE;

 private:
  
  scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner_;

  
  
  scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner_;

  
  scoped_refptr<base::SingleThreadTaskRunner> capture_task_runner_;

  
  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner_;

  
  bool curtain_enabled_;

  
  IPC::Sender* daemon_channel_;

  
  typedef std::map<int, DesktopSessionProxy*> ActiveConnectionsList;
  ActiveConnectionsList active_connections_;

  
  base::WeakPtrFactory<DesktopSessionConnector> connector_factory_;

  
  
  
  int next_id_;

  DISALLOW_COPY_AND_ASSIGN(IpcDesktopEnvironmentFactory);
};

}  

#endif  
