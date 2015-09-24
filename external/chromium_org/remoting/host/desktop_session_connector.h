// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_SESSION_CONNECTOR_H_
#define REMOTING_HOST_DESKTOP_SESSION_CONNECTOR_H_

#include "base/basictypes.h"
#include "base/process/process.h"
#include "ipc/ipc_platform_file.h"

namespace remoting {

class DesktopSessionProxy;
class ScreenResolution;

class DesktopSessionConnector {
 public:
  DesktopSessionConnector() {}
  virtual ~DesktopSessionConnector() {}

  
  
  
  
  virtual void ConnectTerminal(
      DesktopSessionProxy* desktop_session_proxy,
      const ScreenResolution& resolution,
      bool virtual_terminal) = 0;

  
  
  virtual void DisconnectTerminal(
      DesktopSessionProxy* desktop_session_proxy) = 0;

  
  virtual void SetScreenResolution(
      DesktopSessionProxy* desktop_session_proxy,
      const ScreenResolution& resolution) = 0;

  
  
  
  
  virtual void OnDesktopSessionAgentAttached(
      int terminal_id,
      base::ProcessHandle desktop_process,
      IPC::PlatformFileForTransit desktop_pipe) = 0;

  
  
  virtual void OnTerminalDisconnected(int terminal_id) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopSessionConnector);
};

}  

#endif  
