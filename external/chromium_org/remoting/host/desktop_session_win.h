// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_SESSION_WIN_H_
#define REMOTING_HOST_DESKTOP_SESSION_WIN_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "base/win/scoped_handle.h"
#include "ipc/ipc_platform_file.h"
#include "remoting/host/desktop_session.h"
#include "remoting/host/win/wts_terminal_observer.h"
#include "remoting/host/worker_process_ipc_delegate.h"

namespace tracked_objects {
class Location;
}  

namespace remoting {

class AutoThreadTaskRunner;
class DaemonProcess;
class DesktopSession;
class ScreenResolution;
class WorkerProcessLauncher;
class WtsTerminalMonitor;

class DesktopSessionWin
    : public DesktopSession,
      public WorkerProcessIpcDelegate,
      public WtsTerminalObserver {
 public:
  
  static scoped_ptr<DesktopSession> CreateForConsole(
      scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
      scoped_refptr<AutoThreadTaskRunner> io_task_runner,
      DaemonProcess* daemon_process,
      int id,
      const ScreenResolution& resolution);

  
  static scoped_ptr<DesktopSession> CreateForVirtualTerminal(
      scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
      scoped_refptr<AutoThreadTaskRunner> io_task_runner,
      DaemonProcess* daemon_process,
      int id,
      const ScreenResolution& resolution);

 protected:
  
  
  
  DesktopSessionWin(
    scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
    scoped_refptr<AutoThreadTaskRunner> io_task_runner,
    DaemonProcess* daemon_process,
    int id,
    WtsTerminalMonitor* monitor);
  virtual ~DesktopSessionWin();

  const scoped_refptr<AutoThreadTaskRunner>& caller_task_runner() const {
    return caller_task_runner_;
  }

  
  void OnSessionAttachTimeout();

  
  void StartMonitoring(const std::string& terminal_id);

  
  void StopMonitoring();

  
  void TerminateSession();

  
  virtual void InjectSas() = 0;

  
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnPermanentError(int exit_code) OVERRIDE;

  
  virtual void OnSessionAttached(uint32 session_id) OVERRIDE;
  virtual void OnSessionDetached() OVERRIDE;

 private:
  
  void OnDesktopSessionAgentAttached(IPC::PlatformFileForTransit desktop_pipe);

  
  void CrashDesktopProcess(const tracked_objects::Location& location);

  
  void ReportElapsedTime(const std::string& event);

  
  scoped_refptr<AutoThreadTaskRunner> caller_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> io_task_runner_;

  
  base::win::ScopedHandle desktop_process_;

  
  scoped_ptr<WorkerProcessLauncher> launcher_;

  
  WtsTerminalMonitor* monitor_;

  
  bool monitoring_notifications_;

  
  
  base::OneShotTimer<DesktopSessionWin> session_attach_timer_;

  base::Time last_timestamp_;

  DISALLOW_COPY_AND_ASSIGN(DesktopSessionWin);
};

}  

#endif  
