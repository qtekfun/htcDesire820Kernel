// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_WORKER_PROCESS_LAUNCHER_H_
#define REMOTING_HOST_WIN_WORKER_PROCESS_LAUNCHER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "base/win/object_watcher.h"
#include "base/win/scoped_handle.h"
#include "net/base/backoff_entry.h"

namespace base {
class SingleThreadTaskRunner;
class TimeDelta;
} 

namespace IPC {
class Message;
} 

namespace tracked_objects {
class Location;
}  

namespace remoting {

class WorkerProcessIpcDelegate;

class WorkerProcessLauncher
    : public base::NonThreadSafe,
      public base::win::ObjectWatcher::Delegate {
 public:
  class Delegate {
   public:
    virtual ~Delegate();

    
    
    
    virtual void LaunchProcess(WorkerProcessLauncher* event_handler) = 0;

    
    
    virtual void Send(IPC::Message* message) = 0;

    
    virtual void CloseChannel() = 0;

    
    virtual void KillProcess() = 0;
  };

  
  
  
  WorkerProcessLauncher(scoped_ptr<Delegate> launcher_delegate,
                        WorkerProcessIpcDelegate* ipc_handler);
  virtual ~WorkerProcessLauncher();

  
  
  
  
  void Crash(const tracked_objects::Location& location);

  
  
  
  void Send(IPC::Message* message);

  

  
  
  
  void OnProcessLaunched(base::win::ScopedHandle worker_process);

  
  
  
  void OnFatalError();

  
  
  bool OnMessageReceived(const IPC::Message& message);
  void OnChannelConnected(int32 peer_pid);
  void OnChannelError();

 private:
  friend class WorkerProcessLauncherTest;

  
  
  virtual void OnObjectSignaled(HANDLE object) OVERRIDE;

  
  bool stopping() const { return ipc_handler_ == NULL; }

  
  
  void LaunchWorker();

  
  void RecordLaunchResult();

  
  void RecordSuccessfulLaunchForTest();

  
  void SetKillProcessTimeoutForTest(const base::TimeDelta& timeout);

  
  
  void StopWorker();

  
  WorkerProcessIpcDelegate* ipc_handler_;

  
  scoped_ptr<WorkerProcessLauncher::Delegate> launcher_delegate_;

  
  
  DWORD exit_code_;

  
  bool ipc_enabled_;

  
  
  base::OneShotTimer<WorkerProcessLauncher> kill_process_timer_;

  
  base::TimeDelta kill_process_timeout_;

  
  net::BackoffEntry launch_backoff_;

  
  base::OneShotTimer<WorkerProcessLauncher> launch_timer_;

  
  
  base::win::ObjectWatcher process_watcher_;

  
  
  base::OneShotTimer<WorkerProcessLauncher> launch_result_timer_;

  
  base::win::ScopedHandle worker_process_;

  DISALLOW_COPY_AND_ASSIGN(WorkerProcessLauncher);
};

}  

#endif  
