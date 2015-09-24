// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_CHILD_PROCESS_RESOURCE_PROVIDER_H_
#define CHROME_BROWSER_TASK_MANAGER_CHILD_PROCESS_RESOURCE_PROVIDER_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/task_manager/resource_provider.h"
#include "content/public/browser/browser_child_process_observer.h"
#include "content/public/browser/notification_registrar.h"

class TaskManager;

namespace content {
struct ChildProcessData;
}

namespace task_manager {

class ChildProcessResource;

class ChildProcessResourceProvider
    : public ResourceProvider,
      public content::BrowserChildProcessObserver {
 public:
  explicit ChildProcessResourceProvider(TaskManager* task_manager);

  virtual Resource* GetResource(int origin_pid,
                                int render_process_host_id,
                                int routing_id) OVERRIDE;
  virtual void StartUpdating() OVERRIDE;
  virtual void StopUpdating() OVERRIDE;

  
  virtual void BrowserChildProcessHostConnected(
      const content::ChildProcessData& data) OVERRIDE;
  virtual void BrowserChildProcessHostDisconnected(
      const content::ChildProcessData& data) OVERRIDE;

 private:
  virtual ~ChildProcessResourceProvider();

  
  
  virtual void RetrieveChildProcessData();

  
  
  virtual void ChildProcessDataRetreived(
      const std::vector<content::ChildProcessData>& child_processes);

  void AddToTaskManager(const content::ChildProcessData& child_process_data);

  TaskManager* task_manager_;

  
  
  bool updating_;

  
  
  typedef std::map<base::ProcessHandle, ChildProcessResource*>
      ChildProcessMap;
  ChildProcessMap resources_;

  
  
  typedef std::map<int, ChildProcessResource*> PidResourceMap;
  PidResourceMap pid_to_resources_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ChildProcessResourceProvider);
};

}  

#endif  
