// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MACH_BROKER_H_
#define CHROME_BROWSER_MACH_BROKER_H_
#pragma once

#include <map>
#include <string>

#include <mach/mach.h>

#include "base/memory/singleton.h"
#include "base/process.h"
#include "base/process_util.h"
#include "base/synchronization/lock.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class MachBroker : public base::ProcessMetrics::PortProvider,
                   public NotificationObserver {
 public:
  
  static MachBroker* GetInstance();

  
  
  void PrepareForFork();

  struct MachInfo {
    MachInfo() : mach_task_(MACH_PORT_NULL) {}

    MachInfo& SetTask(mach_port_t task) {
      mach_task_ = task;
      return *this;
    }

    mach_port_t mach_task_;
  };

  
  
  
  
  void AddPlaceholderForPid(base::ProcessHandle pid);

  
  
  
  
  void FinalizePid(base::ProcessHandle pid, const MachInfo& mach_info);

  
  void InvalidatePid(base::ProcessHandle pid);

  
  
  base::Lock& GetLock();

  
  
  static std::string GetMachPortName();

  
  virtual mach_port_t TaskForPid(base::ProcessHandle process) const;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);
 private:
  
  MachBroker();
  ~MachBroker();

  
  bool listener_thread_started_;

  
  
  NotificationRegistrar registrar_;

  
  typedef std::map<base::ProcessHandle, MachInfo> MachMap;
  MachMap mach_map_;

  
  mutable base::Lock lock_;

  friend class MachBrokerTest;
  friend class RegisterNotificationTask;
  
  friend struct DefaultSingletonTraits<MachBroker>;
  DISALLOW_COPY_AND_ASSIGN(MachBroker);
};

#endif  

