// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WORKER_HOST_WORKER_SERVICE_H_
#define CONTENT_BROWSER_WORKER_HOST_WORKER_SERVICE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "content/browser/worker_host/worker_process_host.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/worker_service.h"

class GURL;
struct ViewHostMsg_CreateWorker_Params;

namespace content {
class ResourceContext;
class WorkerServiceObserver;
class WorkerStoragePartition;
class WorkerPrioritySetter;

class CONTENT_EXPORT WorkerServiceImpl
    : public NON_EXPORTED_BASE(WorkerService) {
 public:
  
  static WorkerServiceImpl* GetInstance();

  
  void PerformTeardownForTesting();

  
  virtual bool TerminateWorker(int process_id, int route_id) OVERRIDE;
  virtual std::vector<WorkerInfo> GetWorkers() OVERRIDE;
  virtual void AddObserver(WorkerServiceObserver* observer) OVERRIDE;
  virtual void RemoveObserver(WorkerServiceObserver* observer) OVERRIDE;

  
  void CreateWorker(const ViewHostMsg_CreateWorker_Params& params,
                    int route_id,
                    WorkerMessageFilter* filter,
                    ResourceContext* resource_context,
                    const WorkerStoragePartition& worker_partition);
  void LookupSharedWorker(const ViewHostMsg_CreateWorker_Params& params,
                          int route_id,
                          WorkerMessageFilter* filter,
                          ResourceContext* resource_context,
                          const WorkerStoragePartition& worker_partition,
                          bool* exists,
                          bool* url_error);
  void ForwardToWorker(const IPC::Message& message,
                       WorkerMessageFilter* filter);
  void DocumentDetached(unsigned long long document_id,
                        WorkerMessageFilter* filter);

  void OnWorkerMessageFilterClosing(WorkerMessageFilter* filter);

  int next_worker_route_id() { return ++next_worker_route_id_; }

  
  
  
  
  
  bool GetRendererForWorker(int worker_process_id,
                            int* render_process_id,
                            int* render_view_id) const;
  const WorkerProcessHost::WorkerInstance* FindWorkerInstance(
      int worker_process_id);

  void NotifyWorkerDestroyed(
      WorkerProcessHost* process,
      int worker_route_id);

  void NotifyWorkerProcessCreated();

  
  static const int kMaxWorkersWhenSeparate;
  static const int kMaxWorkersPerTabWhenSeparate;

 private:
  friend struct DefaultSingletonTraits<WorkerServiceImpl>;

  WorkerServiceImpl();
  virtual ~WorkerServiceImpl();

  
  bool CreateWorkerFromInstance(WorkerProcessHost::WorkerInstance instance);

  
  
  bool CanCreateWorkerProcess(
      const WorkerProcessHost::WorkerInstance& instance);

  
  
  
  bool TabCanCreateWorkerProcess(
      int render_process_id, int render_route_id, bool* hit_total_worker_limit);

  
  void TryStartingQueuedWorker();

  
  WorkerProcessHost::WorkerInstance* CreatePendingInstance(
      const GURL& url,
      const base::string16& name,
      ResourceContext* resource_context,
      const WorkerStoragePartition& worker_partition);
  WorkerProcessHost::WorkerInstance* FindPendingInstance(
      const GURL& url,
      const base::string16& name,
      const WorkerStoragePartition& worker_partition,
      ResourceContext* resource_context);
  void RemovePendingInstances(
      const GURL& url,
      const base::string16& name,
      const WorkerStoragePartition& worker_partition,
      ResourceContext* resource_context);

  WorkerProcessHost::WorkerInstance* FindSharedWorkerInstance(
      const GURL& url,
      const base::string16& name,
      const WorkerStoragePartition& worker_partition,
      ResourceContext* resource_context);

  scoped_refptr<WorkerPrioritySetter> priority_setter_;

  int next_worker_route_id_;

  WorkerProcessHost::Instances queued_workers_;

  
  
  
  WorkerProcessHost::Instances pending_shared_workers_;

  ObserverList<WorkerServiceObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(WorkerServiceImpl);
};

}  

#endif  
