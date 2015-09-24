// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DAEMON_PROCESS_H_
#define REMOTING_HOST_DAEMON_PROCESS_H_

#include <list>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/process/process.h"
#include "ipc/ipc_channel.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_platform_file.h"
#include "remoting/host/config_watcher.h"
#include "remoting/host/host_status_monitor.h"
#include "remoting/host/worker_process_ipc_delegate.h"

struct SerializedTransportRoute;

namespace tracked_objects {
class Location;
}  

namespace remoting {

class AutoThreadTaskRunner;
class DesktopSession;
class HostEventLogger;
class HostStatusObserver;
class ScreenResolution;

class DaemonProcess
    : public ConfigWatcher::Delegate,
      public HostStatusMonitor,
      public WorkerProcessIpcDelegate {
 public:
  typedef std::list<DesktopSession*> DesktopSessionList;

  virtual ~DaemonProcess();

  
  
  
  
  static scoped_ptr<DaemonProcess> Create(
      scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
      scoped_refptr<AutoThreadTaskRunner> io_task_runner,
      const base::Closure& stopped_callback);

  
  virtual void OnConfigUpdated(const std::string& serialized_config) OVERRIDE;
  virtual void OnConfigWatcherError() OVERRIDE;

  
  virtual void AddStatusObserver(HostStatusObserver* observer) OVERRIDE;
  virtual void RemoveStatusObserver(HostStatusObserver* observer) OVERRIDE;

  
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnPermanentError(int exit_code) OVERRIDE;

  
  
  virtual void SendToNetwork(IPC::Message* message) = 0;

  
  
  
  
  virtual bool OnDesktopSessionAgentAttached(
      int terminal_id,
      base::ProcessHandle desktop_process,
      IPC::PlatformFileForTransit desktop_pipe) = 0;

  
  void CloseDesktopSession(int terminal_id);

 protected:
  DaemonProcess(scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
                scoped_refptr<AutoThreadTaskRunner> io_task_runner,
                const base::Closure& stopped_callback);

  
  void CreateDesktopSession(int terminal_id,
                            const ScreenResolution& resolution,
                            bool virtual_terminal);

  
  
  void SetScreenResolution(int terminal_id, const ScreenResolution& resolution);

  
  void CrashNetworkProcess(const tracked_objects::Location& location);

  
  void Initialize();

  
  void Stop();

  
  
  bool WasTerminalIdAllocated(int terminal_id);

  
  
  void OnAccessDenied(const std::string& jid);
  void OnClientAuthenticated(const std::string& jid);
  void OnClientConnected(const std::string& jid);
  void OnClientDisconnected(const std::string& jid);
  void OnClientRouteChange(const std::string& jid,
                           const std::string& channel_name,
                           const SerializedTransportRoute& route);
  void OnHostStarted(const std::string& xmpp_login);
  void OnHostShutdown();

  
  
  virtual scoped_ptr<DesktopSession> DoCreateDesktopSession(
      int terminal_id,
      const ScreenResolution& resolution,
      bool virtual_terminal) = 0;

  
  virtual void DoCrashNetworkProcess(
      const tracked_objects::Location& location) = 0;

  
  virtual void LaunchNetworkProcess() = 0;

  scoped_refptr<AutoThreadTaskRunner> caller_task_runner() {
    return caller_task_runner_;
  }

  scoped_refptr<AutoThreadTaskRunner> io_task_runner() {
    return io_task_runner_;
  }

  
  friend class DaemonProcessTest;
  const DesktopSessionList& desktop_sessions() const {
    return desktop_sessions_;
  }

 private:
  
  void DeleteAllDesktopSessions();

  
  scoped_refptr<AutoThreadTaskRunner> caller_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> io_task_runner_;

  scoped_ptr<ConfigWatcher> config_watcher_;

  
  std::string serialized_config_;

  
  DesktopSessionList desktop_sessions_;

  
  int next_terminal_id_;

  
  ObserverList<HostStatusObserver> status_observers_;

  
  base::Closure stopped_callback_;

  
  scoped_ptr<HostEventLogger> host_event_logger_;

  base::WeakPtrFactory<DaemonProcess> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DaemonProcess);
};

}  

#endif  
