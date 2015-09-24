// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_HOST_SERVICE_H_
#define REMOTING_HOST_WIN_HOST_SERVICE_H_

#include <windows.h>

#include <list>

#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "remoting/host/win/wts_terminal_monitor.h"

class CommandLine;

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class AutoThreadTaskRunner;
class DaemonProcess;
class WtsTerminalObserver;

class HostService : public WtsTerminalMonitor {
 public:
  static HostService* GetInstance();

  
  bool InitWithCommandLine(const CommandLine* command_line);

  
  int Run();

  
  virtual bool AddWtsTerminalObserver(const std::string& terminal_id,
                                      WtsTerminalObserver* observer) OVERRIDE;
  virtual void RemoveWtsTerminalObserver(
      WtsTerminalObserver* observer) OVERRIDE;

 private:
  HostService();
  ~HostService();

  
  void OnSessionChange(uint32 event, uint32 session_id);

  
  void CreateLauncher(scoped_refptr<AutoThreadTaskRunner> task_runner);

  
  
  int RunAsService();

  
  
  
  void RunAsServiceImpl();

  
  
  int RunInConsole();

  
  void StopDaemonProcess();

  
  bool HandleMessage(UINT message,
                     WPARAM wparam,
                     LPARAM lparam,
                     LRESULT* result);

  static BOOL WINAPI ConsoleControlHandler(DWORD event);

  
  static DWORD WINAPI ServiceControlHandler(DWORD control,
                                            DWORD event_type,
                                            LPVOID event_data,
                                            LPVOID context);

  
  static VOID WINAPI ServiceMain(DWORD argc, WCHAR* argv[]);

  struct RegisteredObserver {
    
    std::string terminal_id;

    
    
    uint32 session_id;

    
    
    WtsTerminalObserver* observer;
  };

  
  std::list<RegisteredObserver> observers_;

  scoped_ptr<DaemonProcess> daemon_process_;

  
  
  scoped_refptr<base::SingleThreadTaskRunner> main_task_runner_;

  
  int (HostService::*run_routine_)();

  
  SERVICE_STATUS_HANDLE service_status_handle_;

  
  base::WaitableEvent stopped_event_;

  
  base::WeakPtrFactory<HostService> weak_factory_;
  base::WeakPtr<HostService> weak_ptr_;

  
  friend struct DefaultSingletonTraits<HostService>;

  DISALLOW_COPY_AND_ASSIGN(HostService);
};

}  

#endif  
