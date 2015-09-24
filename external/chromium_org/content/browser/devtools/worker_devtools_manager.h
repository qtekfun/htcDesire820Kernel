// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_WORKER_DEVTOOLS_MANAGER_H_
#define CONTENT_BROWSER_DEVTOOLS_WORKER_DEVTOOLS_MANAGER_H_

#include <list>
#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "content/browser/worker_host/worker_process_host.h"
#include "content/common/content_export.h"

namespace content {

class DevToolsAgentHost;

class WorkerDevToolsManager {
 public:
  typedef std::pair<int, int> WorkerId;
  class WorkerDevToolsAgentHost;

  
  static WorkerDevToolsManager* GetInstance();

  
  static DevToolsAgentHost* GetDevToolsAgentHostForWorker(
      int worker_process_id,
      int worker_route_id);

  
  static bool HasDevToolsAgentHostForWorker(
      int worker_process_id,
      int worker_route_id);

  void ForwardToDevToolsClient(int worker_process_id,
                               int worker_route_id,
                               const std::string& message);
  void SaveAgentRuntimeState(int worker_process_id,
                             int worker_route_id,
                             const std::string& state);

  
  void WorkerCreated(
      WorkerProcessHost* process,
      const WorkerProcessHost::WorkerInstance& instance);
  void WorkerDestroyed(WorkerProcessHost* process, int worker_route_id);
  void WorkerContextStarted(WorkerProcessHost* process, int worker_route_id);

 private:
  friend struct DefaultSingletonTraits<WorkerDevToolsManager>;
  class DetachedClientHosts;
  struct InspectedWorker;
  typedef std::list<InspectedWorker> InspectedWorkersList;

  WorkerDevToolsManager();
  virtual ~WorkerDevToolsManager();

  void RemoveInspectedWorkerData(const WorkerId& id);
  InspectedWorkersList::iterator FindInspectedWorker(int host_id, int route_id);

  void ConnectDevToolsAgentHostToWorker(int worker_process_id,
                                        int worker_route_id);
  void ForwardToWorkerDevToolsAgent(int worker_process_host_id,
                                    int worker_route_id,
                                    const IPC::Message& message);
  static void ForwardToDevToolsClientOnUIThread(
      int worker_process_id,
      int worker_route_id,
      const std::string& message);
  static void SaveAgentRuntimeStateOnUIThread(
      int worker_process_id,
      int worker_route_id,
      const std::string& state);
  static void NotifyConnectionFailedOnIOThread(int worker_process_id,
                                               int worker_route_id);
  static void NotifyConnectionFailedOnUIThread(int worker_process_id,
                                               int worker_route_id);
  static void SendResumeToWorker(const WorkerId& id);

  InspectedWorkersList inspected_workers_;

  struct TerminatedInspectedWorker;
  typedef std::list<TerminatedInspectedWorker> TerminatedInspectedWorkers;
  
  
  
  
  
  
  
  
  TerminatedInspectedWorkers terminated_workers_;

  typedef std::map<WorkerId, WorkerId> PausedWorkers;
  
  
  
  
  
  
  
  PausedWorkers paused_workers_;

  DISALLOW_COPY_AND_ASSIGN(WorkerDevToolsManager);
};

}  

#endif  
