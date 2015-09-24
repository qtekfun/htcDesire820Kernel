// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef NET_SOCKET_CLIENT_SOCKET_POOL_BASE_H_
#define NET_SOCKET_CLIENT_SOCKET_POOL_BASE_H_

#include <cstddef>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/load_timing_info.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/network_change_notifier.h"
#include "net/base/priority_queue.h"
#include "net/base/request_priority.h"
#include "net/socket/client_socket_pool.h"
#include "net/socket/stream_socket.h"

namespace net {

class ClientSocketHandle;

class NET_EXPORT_PRIVATE ConnectJob {
 public:
  class NET_EXPORT_PRIVATE Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}

    
    
    
    virtual void OnConnectJobComplete(int result,
                                      ConnectJob* job) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  
  ConnectJob(const std::string& group_name,
             base::TimeDelta timeout_duration,
             RequestPriority priority,
             Delegate* delegate,
             const BoundNetLog& net_log);
  virtual ~ConnectJob();

  
  const std::string& group_name() const { return group_name_; }
  const BoundNetLog& net_log() { return net_log_; }

  
  
  
  scoped_ptr<StreamSocket> PassSocket();

  
  
  
  
  
  
  int Connect();

  virtual LoadState GetLoadState() const = 0;

  
  
  
  virtual void GetAdditionalErrorState(ClientSocketHandle* handle) {}

  const LoadTimingInfo::ConnectTiming& connect_timing() const {
    return connect_timing_;
  }

  const BoundNetLog& net_log() const { return net_log_; }

 protected:
  RequestPriority priority() const { return priority_; }
  void SetSocket(scoped_ptr<StreamSocket> socket);
  StreamSocket* socket() { return socket_.get(); }
  void NotifyDelegateOfCompletion(int rv);
  void ResetTimer(base::TimeDelta remainingTime);

  
  LoadTimingInfo::ConnectTiming connect_timing_;

 private:
  virtual int ConnectInternal() = 0;

  void LogConnectStart();
  void LogConnectCompletion(int net_error);

  
  void OnTimeout();

  const std::string group_name_;
  const base::TimeDelta timeout_duration_;
  
  const RequestPriority priority_;
  
  base::OneShotTimer<ConnectJob> timer_;
  Delegate* delegate_;
  scoped_ptr<StreamSocket> socket_;
  BoundNetLog net_log_;
  
  bool idle_;

  DISALLOW_COPY_AND_ASSIGN(ConnectJob);
};

namespace internal {

class NET_EXPORT_PRIVATE ClientSocketPoolBaseHelper
    : public ConnectJob::Delegate,
      public NetworkChangeNotifier::IPAddressObserver {
 public:
  typedef uint32 Flags;

  
  enum Flag {
    NORMAL = 0,  
    NO_IDLE_SOCKETS = 0x1,  
  };

  class NET_EXPORT_PRIVATE Request {
   public:
    Request(ClientSocketHandle* handle,
            const CompletionCallback& callback,
            RequestPriority priority,
            bool ignore_limits,
            Flags flags,
            const BoundNetLog& net_log);

    virtual ~Request();

    ClientSocketHandle* handle() const { return handle_; }
    const CompletionCallback& callback() const { return callback_; }
    RequestPriority priority() const { return priority_; }
    bool ignore_limits() const { return ignore_limits_; }
    Flags flags() const { return flags_; }
    const BoundNetLog& net_log() const { return net_log_; }

   private:
    ClientSocketHandle* const handle_;
    const CompletionCallback callback_;
    
    const RequestPriority priority_;
    const bool ignore_limits_;
    const Flags flags_;
    const BoundNetLog net_log_;

    DISALLOW_COPY_AND_ASSIGN(Request);
  };

  class ConnectJobFactory {
   public:
    ConnectJobFactory() {}
    virtual ~ConnectJobFactory() {}

    virtual scoped_ptr<ConnectJob> NewConnectJob(
        const std::string& group_name,
        const Request& request,
        ConnectJob::Delegate* delegate) const = 0;

    virtual base::TimeDelta ConnectionTimeout() const = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(ConnectJobFactory);
  };

  ClientSocketPoolBaseHelper(
      HigherLayeredPool* pool,
      int max_sockets,
      int max_sockets_per_group,
      base::TimeDelta unused_idle_socket_timeout,
      base::TimeDelta used_idle_socket_timeout,
      ConnectJobFactory* connect_job_factory);

  virtual ~ClientSocketPoolBaseHelper();

  
  
  void AddLowerLayeredPool(LowerLayeredPool* lower_pool);

  
  bool IsStalled() const;

  
  
  void AddHigherLayeredPool(HigherLayeredPool* higher_pool);
  void RemoveHigherLayeredPool(HigherLayeredPool* higher_pool);

  
  int RequestSocket(const std::string& group_name,
                    scoped_ptr<const Request> request);

  
  void RequestSockets(const std::string& group_name,
                      const Request& request,
                      int num_sockets);

  
  void CancelRequest(const std::string& group_name,
                     ClientSocketHandle* handle);

  
  void ReleaseSocket(const std::string& group_name,
                     scoped_ptr<StreamSocket> socket,
                     int id);

  
  void FlushWithError(int error);

  
  void CloseIdleSockets();

  
  int idle_socket_count() const {
    return idle_socket_count_;
  }

  
  
  int IdleSocketCountInGroup(const std::string& group_name) const;

  
  LoadState GetLoadState(const std::string& group_name,
                         const ClientSocketHandle* handle) const;

  base::TimeDelta ConnectRetryInterval() const {
    
    
    return base::TimeDelta::FromMilliseconds(
        ClientSocketPool::kMaxConnectRetryIntervalMs);
  }

  int NumUnassignedConnectJobsInGroup(const std::string& group_name) const {
    return group_map_.find(group_name)->second->unassigned_job_count();
  }

  int NumConnectJobsInGroup(const std::string& group_name) const {
    return group_map_.find(group_name)->second->jobs().size();
  }

  int NumActiveSocketsInGroup(const std::string& group_name) const {
    return group_map_.find(group_name)->second->active_socket_count();
  }

  bool HasGroup(const std::string& group_name) const;

  
  
  
  
  
  static bool cleanup_timer_enabled();
  static bool set_cleanup_timer_enabled(bool enabled);

  
  
  void CleanupIdleSockets(bool force);

  
  
  
  
  
  bool CloseOneIdleSocket();

  
  bool CloseOneIdleConnectionInHigherLayeredPool();

  
  base::DictionaryValue* GetInfoAsValue(const std::string& name,
                                        const std::string& type) const;

  base::TimeDelta ConnectionTimeout() const {
    return connect_job_factory_->ConnectionTimeout();
  }

  static bool connect_backup_jobs_enabled();
  static bool set_connect_backup_jobs_enabled(bool enabled);

  void EnableConnectBackupJobs();

  
  virtual void OnConnectJobComplete(int result, ConnectJob* job) OVERRIDE;

  
  virtual void OnIPAddressChanged() OVERRIDE;

 private:
  friend class base::RefCounted<ClientSocketPoolBaseHelper>;

  
  struct IdleSocket {
    IdleSocket() : socket(NULL) {}

    
    
    
    
    
    
    
    
    bool ShouldCleanup(base::TimeTicks now, base::TimeDelta timeout) const;

    StreamSocket* socket;
    base::TimeTicks start_time;
  };

  typedef PriorityQueue<const Request*> RequestQueue;
  typedef std::map<const ClientSocketHandle*, const Request*> RequestMap;

  
  
  
  class Group {
   public:
    Group();
    ~Group();

    bool IsEmpty() const {
      return active_socket_count_ == 0 && idle_sockets_.empty() &&
          jobs_.empty() && pending_requests_.empty();
    }

    bool HasAvailableSocketSlot(int max_sockets_per_group) const {
      return NumActiveSocketSlots() < max_sockets_per_group;
    }

    int NumActiveSocketSlots() const {
      return active_socket_count_ + static_cast<int>(jobs_.size()) +
          static_cast<int>(idle_sockets_.size());
    }

    bool IsStalledOnPoolMaxSockets(int max_sockets_per_group) const {
      return HasAvailableSocketSlot(max_sockets_per_group) &&
          pending_requests_.size() > jobs_.size();
    }

    
    
    
    
    RequestPriority TopPendingPriority() const {
      
      
      return pending_requests_.FirstMax().value()->priority();
    }

    
    
    void StartBackupJobTimer(const std::string& group_name,
                             ClientSocketPoolBaseHelper* pool);

    bool BackupJobTimerIsRunning() const;

    
    
    
    bool TryToUseUnassignedConnectJob();

    void AddJob(scoped_ptr<ConnectJob> job, bool is_preconnect);
    
    void RemoveJob(ConnectJob* job);
    void RemoveAllJobs();

    bool has_pending_requests() const {
      return !pending_requests_.empty();
    }

    size_t pending_request_count() const {
      return pending_requests_.size();
    }

    
    
    const Request* GetNextPendingRequest() const;

    
    bool HasConnectJobForHandle(const ClientSocketHandle* handle) const;

    
    
    
    void InsertPendingRequest(scoped_ptr<const Request> request);

    
    
    scoped_ptr<const Request> PopNextPendingRequest();

    
    
    scoped_ptr<const Request> FindAndRemovePendingRequest(
        ClientSocketHandle* handle);

    void IncrementActiveSocketCount() { active_socket_count_++; }
    void DecrementActiveSocketCount() { active_socket_count_--; }

    int unassigned_job_count() const { return unassigned_job_count_; }
    const std::set<ConnectJob*>& jobs() const { return jobs_; }
    const std::list<IdleSocket>& idle_sockets() const { return idle_sockets_; }
    int active_socket_count() const { return active_socket_count_; }
    std::list<IdleSocket>* mutable_idle_sockets() { return &idle_sockets_; }

   private:
    
    
    scoped_ptr<const Request> RemovePendingRequest(
        const RequestQueue::Pointer& pointer);

    
    void OnBackupJobTimerFired(
        std::string group_name,
        ClientSocketPoolBaseHelper* pool);

    
    
    void SanityCheck();

    
    
    
    
    
    
    size_t unassigned_job_count_;

    std::list<IdleSocket> idle_sockets_;
    std::set<ConnectJob*> jobs_;
    RequestQueue pending_requests_;
    int active_socket_count_;  
    
    base::OneShotTimer<Group> backup_job_timer_;
  };

  typedef std::map<std::string, Group*> GroupMap;

  typedef std::set<ConnectJob*> ConnectJobSet;

  struct CallbackResultPair {
    CallbackResultPair();
    CallbackResultPair(const CompletionCallback& callback_in, int result_in);
    ~CallbackResultPair();

    CompletionCallback callback;
    int result;
  };

  typedef std::map<const ClientSocketHandle*, CallbackResultPair>
      PendingCallbackMap;

  Group* GetOrCreateGroup(const std::string& group_name);
  void RemoveGroup(const std::string& group_name);
  void RemoveGroup(GroupMap::iterator it);

  
  void IncrementIdleCount();
  void DecrementIdleCount();

  
  void StartIdleSocketTimer();

  
  
  
  
  bool FindTopStalledGroup(Group** group, std::string* group_name) const;

  
  
  void OnCleanupTimerFired() {
    CleanupIdleSockets(false);
  }

  
  void RemoveConnectJob(ConnectJob* job, Group* group);

  
  void OnAvailableSocketSlot(const std::string& group_name, Group* group);

  
  void ProcessPendingRequest(const std::string& group_name, Group* group);

  
  void HandOutSocket(scoped_ptr<StreamSocket> socket,
                     bool reused,
                     const LoadTimingInfo::ConnectTiming& connect_timing,
                     ClientSocketHandle* handle,
                     base::TimeDelta time_idle,
                     Group* group,
                     const BoundNetLog& net_log);

  
  void AddIdleSocket(scoped_ptr<StreamSocket> socket, Group* group);

  
  
  void CancelAllConnectJobs();

  
  
  void CancelAllRequestsWithError(int error);

  
  bool ReachedMaxSocketsLimit() const;

  
  
  
  int RequestSocketInternal(const std::string& group_name,
                            const Request& request);

  
  
  bool AssignIdleSocketToRequest(const Request& request, Group* group);

  static void LogBoundConnectJobToRequest(
      const NetLog::Source& connect_job_source, const Request& request);

  
  
  
  bool CloseOneIdleSocketExceptInGroup(const Group* group);

  
  
  void CheckForStalledSocketGroups();

  
  
  
  void InvokeUserCallbackLater(
      ClientSocketHandle* handle, const CompletionCallback& callback, int rv);

  
  
  
  
  void InvokeUserCallback(ClientSocketHandle* handle);

  
  
  void TryToCloseSocketsInLayeredPools();

  GroupMap group_map_;

  
  
  
  PendingCallbackMap pending_callback_map_;

  
  
  base::RepeatingTimer<ClientSocketPoolBaseHelper> timer_;

  
  int idle_socket_count_;

  
  int connecting_socket_count_;

  
  int handed_out_socket_count_;

  
  const int max_sockets_;

  
  const int max_sockets_per_group_;

  
  bool use_cleanup_timer_;

  
  const base::TimeDelta unused_idle_socket_timeout_;
  const base::TimeDelta used_idle_socket_timeout_;

  const scoped_ptr<ConnectJobFactory> connect_job_factory_;

  
  bool connect_backup_jobs_enabled_;

  
  
  
  int pool_generation_number_;

  
  
  HigherLayeredPool* pool_;

  
  
  std::set<HigherLayeredPool*> higher_pools_;

  
  
  
  std::set<LowerLayeredPool*> lower_pools_;

  base::WeakPtrFactory<ClientSocketPoolBaseHelper> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ClientSocketPoolBaseHelper);
};

}  

template <typename SocketParams>
class ClientSocketPoolBase {
 public:
  class Request : public internal::ClientSocketPoolBaseHelper::Request {
   public:
    Request(ClientSocketHandle* handle,
            const CompletionCallback& callback,
            RequestPriority priority,
            internal::ClientSocketPoolBaseHelper::Flags flags,
            bool ignore_limits,
            const scoped_refptr<SocketParams>& params,
            const BoundNetLog& net_log)
        : internal::ClientSocketPoolBaseHelper::Request(
              handle, callback, priority, ignore_limits, flags, net_log),
          params_(params) {}

    const scoped_refptr<SocketParams>& params() const { return params_; }

   private:
    const scoped_refptr<SocketParams> params_;
  };

  class ConnectJobFactory {
   public:
    ConnectJobFactory() {}
    virtual ~ConnectJobFactory() {}

    virtual scoped_ptr<ConnectJob> NewConnectJob(
        const std::string& group_name,
        const Request& request,
        ConnectJob::Delegate* delegate) const = 0;

    virtual base::TimeDelta ConnectionTimeout() const = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(ConnectJobFactory);
  };

  
  
  
  
  
  
  ClientSocketPoolBase(
      HigherLayeredPool* self,
      int max_sockets,
      int max_sockets_per_group,
      ClientSocketPoolHistograms* histograms,
      base::TimeDelta unused_idle_socket_timeout,
      base::TimeDelta used_idle_socket_timeout,
      ConnectJobFactory* connect_job_factory)
      : histograms_(histograms),
        helper_(self, max_sockets, max_sockets_per_group,
                unused_idle_socket_timeout, used_idle_socket_timeout,
                new ConnectJobFactoryAdaptor(connect_job_factory)) {}

  virtual ~ClientSocketPoolBase() {}

  
  void AddLowerLayeredPool(LowerLayeredPool* lower_pool) {
    helper_.AddLowerLayeredPool(lower_pool);
  }

  void AddHigherLayeredPool(HigherLayeredPool* higher_pool) {
    helper_.AddHigherLayeredPool(higher_pool);
  }

  void RemoveHigherLayeredPool(HigherLayeredPool* higher_pool) {
    helper_.RemoveHigherLayeredPool(higher_pool);
  }

  
  
  int RequestSocket(const std::string& group_name,
                    const scoped_refptr<SocketParams>& params,
                    RequestPriority priority,
                    ClientSocketHandle* handle,
                    const CompletionCallback& callback,
                    const BoundNetLog& net_log) {
    scoped_ptr<const Request> request(
        new Request(handle, callback, priority,
                    internal::ClientSocketPoolBaseHelper::NORMAL,
                    params->ignore_limits(),
                    params, net_log));
    return helper_.RequestSocket(
        group_name,
        request.template PassAs<
            const internal::ClientSocketPoolBaseHelper::Request>());
  }

  
  
  
  void RequestSockets(const std::string& group_name,
                      const scoped_refptr<SocketParams>& params,
                      int num_sockets,
                      const BoundNetLog& net_log) {
    const Request request(NULL ,
                          CompletionCallback(),
                          DEFAULT_PRIORITY,
                          internal::ClientSocketPoolBaseHelper::NO_IDLE_SOCKETS,
                          params->ignore_limits(),
                          params,
                          net_log);
    helper_.RequestSockets(group_name, request, num_sockets);
  }

  void CancelRequest(const std::string& group_name,
                     ClientSocketHandle* handle) {
    return helper_.CancelRequest(group_name, handle);
  }

  void ReleaseSocket(const std::string& group_name,
                     scoped_ptr<StreamSocket> socket,
                     int id) {
    return helper_.ReleaseSocket(group_name, socket.Pass(), id);
  }

  void FlushWithError(int error) { helper_.FlushWithError(error); }

  bool IsStalled() const { return helper_.IsStalled(); }

  void CloseIdleSockets() { return helper_.CloseIdleSockets(); }

  int idle_socket_count() const { return helper_.idle_socket_count(); }

  int IdleSocketCountInGroup(const std::string& group_name) const {
    return helper_.IdleSocketCountInGroup(group_name);
  }

  LoadState GetLoadState(const std::string& group_name,
                         const ClientSocketHandle* handle) const {
    return helper_.GetLoadState(group_name, handle);
  }

  virtual void OnConnectJobComplete(int result, ConnectJob* job) {
    return helper_.OnConnectJobComplete(result, job);
  }

  int NumUnassignedConnectJobsInGroup(const std::string& group_name) const {
    return helper_.NumUnassignedConnectJobsInGroup(group_name);
  }

  int NumConnectJobsInGroup(const std::string& group_name) const {
    return helper_.NumConnectJobsInGroup(group_name);
  }

  int NumActiveSocketsInGroup(const std::string& group_name) const {
    return helper_.NumActiveSocketsInGroup(group_name);
  }

  bool HasGroup(const std::string& group_name) const {
    return helper_.HasGroup(group_name);
  }

  void CleanupIdleSockets(bool force) {
    return helper_.CleanupIdleSockets(force);
  }

  base::DictionaryValue* GetInfoAsValue(const std::string& name,
                                        const std::string& type) const {
    return helper_.GetInfoAsValue(name, type);
  }

  base::TimeDelta ConnectionTimeout() const {
    return helper_.ConnectionTimeout();
  }

  ClientSocketPoolHistograms* histograms() const {
    return histograms_;
  }

  void EnableConnectBackupJobs() { helper_.EnableConnectBackupJobs(); }

  bool CloseOneIdleSocket() { return helper_.CloseOneIdleSocket(); }

  bool CloseOneIdleConnectionInHigherLayeredPool() {
    return helper_.CloseOneIdleConnectionInHigherLayeredPool();
  }

 private:
  
  
  
  
  
  class ConnectJobFactoryAdaptor
      : public internal::ClientSocketPoolBaseHelper::ConnectJobFactory {
   public:
    typedef typename ClientSocketPoolBase<SocketParams>::ConnectJobFactory
        ConnectJobFactory;

    explicit ConnectJobFactoryAdaptor(ConnectJobFactory* connect_job_factory)
        : connect_job_factory_(connect_job_factory) {}
    virtual ~ConnectJobFactoryAdaptor() {}

    virtual scoped_ptr<ConnectJob> NewConnectJob(
        const std::string& group_name,
        const internal::ClientSocketPoolBaseHelper::Request& request,
        ConnectJob::Delegate* delegate) const OVERRIDE {
      const Request& casted_request = static_cast<const Request&>(request);
      return connect_job_factory_->NewConnectJob(
          group_name, casted_request, delegate);
    }

    virtual base::TimeDelta ConnectionTimeout() const {
      return connect_job_factory_->ConnectionTimeout();
    }

    const scoped_ptr<ConnectJobFactory> connect_job_factory_;
  };

  
  ClientSocketPoolHistograms* const histograms_;
  internal::ClientSocketPoolBaseHelper helper_;

  DISALLOW_COPY_AND_ASSIGN(ClientSocketPoolBase);
};

}  

#endif  
