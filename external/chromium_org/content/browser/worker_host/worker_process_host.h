// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WORKER_HOST_WORKER_PROCESS_HOST_H_
#define CONTENT_BROWSER_WORKER_HOST_WORKER_PROCESS_HOST_H_

#include <list>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/worker_host/worker_document_set.h"
#include "content/browser/worker_host/worker_storage_partition.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_child_process_host_delegate.h"
#include "content/public/browser/browser_child_process_host_iterator.h"
#include "content/public/common/process_type.h"
#include "ipc/ipc_sender.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"

struct ResourceHostMsg_Request;

namespace fileapi {
class FileSystemContext;
}  

namespace net {
class URLRequestContext;
}

namespace webkit_database {
class DatabaseTracker;
}  

namespace content {
class BrowserChildProcessHostImpl;
class IndexedDBContextImpl;
class ResourceContext;
class SocketStreamDispatcherHost;
class WorkerServiceImpl;

class WorkerProcessHost : public BrowserChildProcessHostDelegate,
                          public IPC::Sender {
 public:
  
  
  class WorkerInstance {
   public:
    WorkerInstance(const GURL& url,
                   const base::string16& name,
                   int worker_route_id,
                   int parent_process_id,
                   int64 main_resource_appcache_id,
                   ResourceContext* resource_context,
                   const WorkerStoragePartition& partition);
    
    WorkerInstance(const GURL& url,
                   bool shared,
                   const base::string16& name,
                   ResourceContext* resource_context,
                   const WorkerStoragePartition& partition);
    ~WorkerInstance();

    
    typedef std::pair<WorkerMessageFilter*, int> FilterInfo;

    
    void AddFilter(WorkerMessageFilter* filter, int route_id);
    void RemoveFilter(WorkerMessageFilter* filter, int route_id);
    void RemoveFilters(WorkerMessageFilter* filter);
    bool HasFilter(WorkerMessageFilter* filter, int route_id) const;
    bool RendererIsParent(int render_process_id, int render_view_id) const;
    int NumFilters() const { return filters_.size(); }
    
    FilterInfo GetFilter() const;

    typedef std::list<FilterInfo> FilterList;
    const FilterList& filters() const { return filters_; }

    
    
    
    bool Matches(
        const GURL& url,
        const base::string16& name,
        const WorkerStoragePartition& partition,
        ResourceContext* resource_context) const;

    
    
    
    void ShareDocumentSet(const WorkerInstance& instance) {
      worker_document_set_ = instance.worker_document_set_;
    };

    
    bool closed() const { return closed_; }
    void set_closed(bool closed) { closed_ = closed; }
    const GURL& url() const { return url_; }
    const base::string16 name() const { return name_; }
    int worker_route_id() const { return worker_route_id_; }
    int parent_process_id() const { return parent_process_id_; }
    int64 main_resource_appcache_id() const {
      return main_resource_appcache_id_;
    }
    WorkerDocumentSet* worker_document_set() const {
      return worker_document_set_.get();
    }
    ResourceContext* resource_context() const {
      return resource_context_;
    }
    const WorkerStoragePartition& partition() const {
      return partition_;
    }

   private:
    
    GURL url_;
    bool closed_;
    base::string16 name_;
    int worker_route_id_;
    int parent_process_id_;
    int64 main_resource_appcache_id_;
    FilterList filters_;
    scoped_refptr<WorkerDocumentSet> worker_document_set_;
    ResourceContext* const resource_context_;
    WorkerStoragePartition partition_;
  };

  WorkerProcessHost(ResourceContext* resource_context,
                    const WorkerStoragePartition& partition);
  virtual ~WorkerProcessHost();

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  
  bool Init(int render_process_id);

  
  void CreateWorker(const WorkerInstance& instance);

  
  
  bool FilterMessage(const IPC::Message& message, WorkerMessageFilter* filter);

  void FilterShutdown(WorkerMessageFilter* filter);

  
  
  void DocumentDetached(WorkerMessageFilter* filter,
                        unsigned long long document_id);

  
  CONTENT_EXPORT void TerminateWorker(int worker_route_id);

  
  void SetBackgrounded(bool backgrounded);

  CONTENT_EXPORT const ChildProcessData& GetData();

  typedef std::list<WorkerInstance> Instances;
  const Instances& instances() const { return instances_; }

  ResourceContext* resource_context() const {
    return resource_context_;
  }

  bool process_launched() const { return process_launched_; }

 protected:
  friend class WorkerServiceImpl;

  Instances& mutable_instances() { return instances_; }

 private:
  
  virtual void OnProcessLaunched() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void CreateMessageFilters(int render_process_id);

  void OnWorkerContextClosed(int worker_route_id);
  void OnAllowDatabase(int worker_route_id,
                       const GURL& url,
                       const base::string16& name,
                       const base::string16& display_name,
                       unsigned long estimated_size,
                       bool* result);
  void OnAllowFileSystem(int worker_route_id,
                         const GURL& url,
                         bool* result);
  void OnAllowIndexedDB(int worker_route_id,
                        const GURL& url,
                        const base::string16& name,
                        bool* result);
  void OnForceKillWorkerProcess();

  
  
  void RelayMessage(const IPC::Message& message,
                    WorkerMessageFilter* filter,
                    int route_id);

  void ShutdownSocketStreamDispatcherHostIfNecessary();

  virtual bool CanShutdown() OVERRIDE;

  
  void UpdateTitle();

  
  
  std::vector<std::pair<int, int> > GetRenderViewIDsForWorker(int route_id);

  
  void GetContexts(const ResourceHostMsg_Request& request,
                   ResourceContext** resource_context,
                   net::URLRequestContext** request_context);
  net::URLRequestContext* GetRequestContext(ResourceType::Type resource_type);

  Instances instances_;

  ResourceContext* const resource_context_;
  WorkerStoragePartition partition_;

  
  
  
  scoped_refptr<WorkerMessageFilter> worker_message_filter_;

  scoped_ptr<BrowserChildProcessHostImpl> process_;
  bool process_launched_;

  scoped_refptr<SocketStreamDispatcherHost> socket_stream_dispatcher_host_;

  DISALLOW_COPY_AND_ASSIGN(WorkerProcessHost);
};

class WorkerProcessHostIterator
    : public BrowserChildProcessHostTypeIterator<WorkerProcessHost> {
 public:
  WorkerProcessHostIterator()
      : BrowserChildProcessHostTypeIterator<WorkerProcessHost>(
            PROCESS_TYPE_WORKER) {
  }
};

}  

#endif  
