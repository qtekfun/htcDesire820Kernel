// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MACH_BROKER_MAC_H_
#define CONTENT_BROWSER_MACH_BROKER_MAC_H_

#include <map>
#include <string>

#include <mach/mach.h>

#include "base/memory/singleton.h"
#include "base/process/process_handle.h"
#include "base/process/process_metrics.h"
#include "base/synchronization/lock.h"
#include "content/public/browser/browser_child_process_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace content {

class CONTENT_EXPORT MachBroker : public base::ProcessMetrics::PortProvider,
                                  public BrowserChildProcessObserver,
                                  public NotificationObserver {
 public:
  
  
  
  
  static bool ChildSendTaskPortToParent();

  
  static MachBroker* GetInstance();

  
  
  
  base::Lock& GetLock();

  
  
  
  void EnsureRunning();

  
  
  
  
  void AddPlaceholderForPid(base::ProcessHandle pid);

  
  virtual mach_port_t TaskForPid(base::ProcessHandle process) const OVERRIDE;

  
  virtual void BrowserChildProcessHostDisconnected(
      const ChildProcessData& data) OVERRIDE;
  virtual void BrowserChildProcessCrashed(
      const ChildProcessData& data) OVERRIDE;

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;
 private:
  friend class MachBrokerTest;
  friend class MachListenerThreadDelegate;
  friend struct DefaultSingletonTraits<MachBroker>;

  MachBroker();
  virtual ~MachBroker();

  
  
  
  
  void FinalizePid(base::ProcessHandle pid, mach_port_t task_port);

  
  void InvalidatePid(base::ProcessHandle pid);

  
  
  static std::string GetMachPortName();
  
  void RegisterNotifications();

  
  bool listener_thread_started_;

  
  
  NotificationRegistrar registrar_;

  
  typedef std::map<base::ProcessHandle, mach_port_t> MachMap;
  MachMap mach_map_;

  
  mutable base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(MachBroker);
};

}  

#endif  
