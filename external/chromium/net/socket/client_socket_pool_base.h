/*
 * Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Copyright (c) 2011,2012 The Linux Foundation. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * A ClientSocketPoolBase is used to restrict the number of sockets open at
 * a time.  It also maintains a list of idle persistent sockets for reuse.
 * Subclasses of ClientSocketPool should compose ClientSocketPoolBase to handle
 * the core logic of (1) restricting the number of active (connected or
 * connecting) sockets per "group" (generally speaking, the hostname), (2)
 * maintaining a per-group list of idle, persistent sockets for reuse, and (3)
 * limiting the total number of active sockets in the system.
 *
 * ClientSocketPoolBase abstracts socket connection details behind ConnectJob,
 * ConnectJobFactory, and SocketParams.  When a socket "slot" becomes available,
 * the ClientSocketPoolBase will ask the ConnectJobFactory to create a
 * ConnectJob with a SocketParams.  Subclasses of ClientSocketPool should
 * implement their socket specific connection by subclassing ConnectJob and
 * implementing ConnectJob::ConnectInternal().  They can control the parameters
 * passed to each new ConnectJob instance via their ConnectJobFactory subclass
 * and templated SocketParams parameter.
 */
#ifndef NET_SOCKET_CLIENT_SOCKET_POOL_BASE_H_
#define NET_SOCKET_CLIENT_SOCKET_POOL_BASE_H_
#pragma once

#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "base/time.h"
#include "base/timer.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/base/network_change_notifier.h"
#include "net/base/request_priority.h"
#include "net/socket/client_socket.h"
#include "net/socket/client_socket_pool.h"
#include "net/base/host_resolver.h"
#include "tcp_fin_aggregation.h"
#include "tcp_fin_aggregation_factory.h"

namespace net {

class ClientSocketHandle;
class HttpNetworkSession;
class ITCPFinAggregation;

class ConnectJob {
 public:
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}

    
    virtual void OnConnectJobComplete(int result, ConnectJob* job) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  
  ConnectJob(const std::string& group_name,
             base::TimeDelta timeout_duration,
             Delegate* delegate,
             const BoundNetLog& net_log);
  virtual ~ConnectJob();

  
  const std::string& group_name() const { return group_name_; }
  const BoundNetLog& net_log() { return net_log_; }
  bool is_preconnect() const { return preconnect_state_ != NOT_PRECONNECT; }
  bool is_unused_preconnect() const {
    return preconnect_state_ == UNUSED_PRECONNECT;
  }

  
  
  
  
  void Initialize(bool is_preconnect);

  
  
  ClientSocket* ReleaseSocket() { return socket_.release(); }

  
  
  
  
  
  
  int Connect();

  
  
  void UseForNormalRequest();

  virtual LoadState GetLoadState() const = 0;

  
  
  
  virtual void GetAdditionalErrorState(ClientSocketHandle* handle) {}

  const BoundNetLog& net_log() const { return net_log_; }

 protected:
  void set_socket(ClientSocket* socket);
  ClientSocket* socket() { return socket_.get(); }
  void NotifyDelegateOfCompletion(int rv);
  void ResetTimer(base::TimeDelta remainingTime);

 private:
  enum PreconnectState {
    NOT_PRECONNECT,
    UNUSED_PRECONNECT,
    USED_PRECONNECT,
  };

  virtual int ConnectInternal() = 0;

  void LogConnectStart();
  void LogConnectCompletion(int net_error);

  
  void OnTimeout();

  const std::string group_name_;
  const base::TimeDelta timeout_duration_;
  
  base::OneShotTimer<ConnectJob> timer_;
  Delegate* delegate_;
  scoped_ptr<ClientSocket> socket_;
  BoundNetLog net_log_;
  
  bool idle_;
  PreconnectState preconnect_state_;

  DISALLOW_COPY_AND_ASSIGN(ConnectJob);
};

namespace internal {

  
  class IdleSocket {
  public:
    IdleSocket() : socket(NULL) {}
    ClientSocket* socket;
    base::Time start_time;

    
    
    
    
    
    
    
    
    bool ShouldCleanup(base::Time now, base::TimeDelta timeout) const;

    base::Time StartTime() const
    {
      return start_time;
    }
  };

class ClientSocketPoolBaseHelper
    : public ConnectJob::Delegate,
      public NetworkChangeNotifier::IPAddressObserver {
 public:
  typedef uint32 Flags;

  
  enum Flag {
    NORMAL = 0,  
    NO_IDLE_SOCKETS = 0x1,  
  };

  class Request {
   public:
    Request(ClientSocketHandle* handle,
            CompletionCallback* callback,
            RequestPriority priority,
            bool ignore_limits,
            Flags flags,
            const BoundNetLog& net_log
#ifdef ANDROID
            , bool valid_uid, uid_t calling_uid
#endif
            );

    virtual ~Request();

    ClientSocketHandle* handle() const { return handle_; }
    CompletionCallback* callback() const { return callback_; }
    RequestPriority priority() const { return priority_; }
    bool ignore_limits() const { return ignore_limits_; }
    Flags flags() const { return flags_; }
    const BoundNetLog& net_log() const { return net_log_; }

   private:
    ClientSocketHandle* const handle_;
    CompletionCallback* const callback_;
    const RequestPriority priority_;
    bool ignore_limits_;
    const Flags flags_;
    BoundNetLog net_log_;
#ifdef ANDROID
    bool valid_uid_;
    uid_t calling_uid_;
#endif

    DISALLOW_COPY_AND_ASSIGN(Request);
  };

  class ConnectJobFactory {
   public:
    ConnectJobFactory() {}
    virtual ~ConnectJobFactory() {}

    virtual ConnectJob* NewConnectJob(
        const std::string& group_name,
        const Request& request,
        ConnectJob::Delegate* delegate) const = 0;

    virtual base::TimeDelta ConnectionTimeout() const = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(ConnectJobFactory);
  };

  ClientSocketPoolBaseHelper(
      int max_sockets,
      int max_sockets_per_group,
      base::TimeDelta unused_idle_socket_timeout,
      base::TimeDelta used_idle_socket_timeout,
      ConnectJobFactory* connect_job_factory,
      HttpNetworkSession *network_session);

  ~ClientSocketPoolBaseHelper();

  
  
  
  int RequestSocket(const std::string& group_name, const Request* request);

  
  void RequestSockets(const std::string& group_name,
                      const Request& request,
                      int num_sockets);

  
  void CancelRequest(const std::string& group_name,
                     ClientSocketHandle* handle);

  
  void ReleaseSocket(const std::string& group_name,
                     ClientSocket* socket,
                     int id);

  
  void Flush();

  
  void CloseIdleSockets();

  
  int idle_socket_count() const {
    return idle_socket_count_;
  }

  
  void IncrementIdleCount();
  void DecrementIdleCount();

  class Group;
  typedef std::map<std::string, Group*> GroupMap;

  void RemoveGroup(const std::string& group_name);
  void RemoveGroup(GroupMap::iterator it);

  GroupMap group_map_;

  
  
  int IdleSocketCountInGroup(const std::string& group_name) const;

  
  LoadState GetLoadState(const std::string& group_name,
                         const ClientSocketHandle* handle) const;

  int ConnectRetryIntervalMs() const {
    
    
    return ClientSocketPool::kMaxConnectRetryIntervalMs;
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

  
  DictionaryValue* GetInfoAsValue(const std::string& name,
                                  const std::string& type) const;

  base::TimeDelta ConnectionTimeout() const {
    return connect_job_factory_->ConnectionTimeout();
  }

  static bool connect_backup_jobs_enabled();
  static bool set_connect_backup_jobs_enabled(bool enabled);

  void EnableConnectBackupJobs();

  
  virtual void OnConnectJobComplete(int result, ConnectJob* job);

  
  virtual void OnIPAddressChanged();

  typedef std::deque<const Request* > RequestQueue;

  HttpNetworkSession *network_session_;

  
  
  
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

    bool IsStalled(int max_sockets_per_group) const {
      return HasAvailableSocketSlot(max_sockets_per_group) &&
          pending_requests_.size() > jobs_.size();
    }

    RequestPriority TopPendingPriority() const {
      return pending_requests_.front()->priority();
    }

    bool HasBackupJob() const { return !method_factory_.empty(); }

    void CleanupBackupJob() {
      method_factory_.RevokeAll();
    }

    
    void StartBackupSocketTimer(const std::string& group_name,
                                ClientSocketPoolBaseHelper* pool);

    
    
    bool TryToUsePreconnectConnectJob();

    void AddJob(ConnectJob* job) { jobs_.insert(job); }
    void RemoveJob(ConnectJob* job) { jobs_.erase(job); }
    void RemoveAllJobs();

    void IncrementActiveSocketCount() { active_socket_count_++; }
    void DecrementActiveSocketCount() { active_socket_count_--; }

    const std::set<ConnectJob*>& jobs() const { return jobs_; }
    const std::list<IdleSocket>& idle_sockets() const { return idle_sockets_; }
    const RequestQueue& pending_requests() const { return pending_requests_; }
    int active_socket_count() const { return active_socket_count_; }
    RequestQueue* mutable_pending_requests() { return &pending_requests_; }
    std::list<IdleSocket>* mutable_idle_sockets() { return &idle_sockets_; }

   private:
    
    void OnBackupSocketTimerFired(
        std::string group_name,
        ClientSocketPoolBaseHelper* pool);

    std::list<IdleSocket> idle_sockets_;
    std::set<ConnectJob*> jobs_;
    RequestQueue pending_requests_;
    int active_socket_count_;  
    
    ScopedRunnableMethodFactory<Group> method_factory_;
  };

 private:
  friend class base::RefCounted<ClientSocketPoolBaseHelper>;

  typedef std::map<const ClientSocketHandle*, const Request*> RequestMap;

  typedef std::set<ConnectJob*> ConnectJobSet;

  struct CallbackResultPair {
    CallbackResultPair() : callback(NULL), result(OK) {}
    CallbackResultPair(CompletionCallback* callback_in, int result_in)
        : callback(callback_in), result(result_in) {}

    CompletionCallback* callback;
    int result;
  };

  typedef std::map<const ClientSocketHandle*, CallbackResultPair>
      PendingCallbackMap;

  static void InsertRequestIntoQueue(const Request* r,
                                     RequestQueue* pending_requests);
  static const Request* RemoveRequestFromQueue(const RequestQueue::iterator& it,
                                               Group* group);

  Group* GetOrCreateGroup(const std::string& group_name);

  
  void StartIdleSocketTimer();

  
  
  
  
  bool FindTopStalledGroup(Group** group, std::string* group_name);

  
  
  void OnCleanupTimerFired();

  
  void RemoveConnectJob(ConnectJob* job, Group* group);

  
  void OnAvailableSocketSlot(const std::string& group_name, Group* group);

  
  void ProcessPendingRequest(const std::string& group_name, Group* group);

  
  void HandOutSocket(ClientSocket* socket,
                     bool reused,
                     ClientSocketHandle* handle,
                     base::TimeDelta time_idle,
                     Group* group,
                     const BoundNetLog& net_log);

  
  void AddIdleSocket(ClientSocket* socket, Group* group);

  
  
  void CancelAllConnectJobs();

  
  
  void AbortAllRequests();

  
  bool ReachedMaxSocketsLimit() const;

  
  
  
  int RequestSocketInternal(const std::string& group_name,
                            const Request* request);

  
  
  
  bool AssignIdleSocketToGroup(const Request* request, Group* group);

  static void LogBoundConnectJobToRequest(
      const NetLog::Source& connect_job_source, const Request* request);

  
  
  
  
  
  void CloseOneIdleSocket();

  
  
  
  bool CloseOneIdleSocketExceptInGroup(const Group* group);

  
  
  void CheckForStalledSocketGroups();

  
  
  
  void InvokeUserCallbackLater(
      ClientSocketHandle* handle, CompletionCallback* callback, int rv);

  
  
  
  
  void InvokeUserCallback(ClientSocketHandle* handle);

  
  
  
  PendingCallbackMap pending_callback_map_;

  
  
  base::RepeatingTimer<ClientSocketPoolBaseHelper> timer_;

  
  int idle_socket_count_;

  
  int connecting_socket_count_;

  
  int handed_out_socket_count_;

  
  const int max_sockets_;

  
  const int max_sockets_per_group_;

  
  
  ITCPFinAggregation* tcp_fin_aggregation;

  
  bool net_tcp_fin_aggr_feature_enabled_sys_property_;

  
  bool use_cleanup_timer_;

  
  bool net_statistics_enabled;

  
  bool close_unused_sockets_enabled;

  
  const base::TimeDelta unused_idle_socket_timeout_;
  const base::TimeDelta used_idle_socket_timeout_;

  const scoped_ptr<ConnectJobFactory> connect_job_factory_;

  
  bool connect_backup_jobs_enabled_;

  
  
  
  int pool_generation_number_;

  ScopedRunnableMethodFactory<ClientSocketPoolBaseHelper> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(ClientSocketPoolBaseHelper);
};

}  

static const int kUsedIdleSocketTimeout = 2;

template <typename SocketParams>
class ClientSocketPoolBase {
 public:
  class Request : public internal::ClientSocketPoolBaseHelper::Request {
   public:
    Request(ClientSocketHandle* handle,
            CompletionCallback* callback,
            RequestPriority priority,
            internal::ClientSocketPoolBaseHelper::Flags flags,
            bool ignore_limits,
            const scoped_refptr<SocketParams>& params,
            const BoundNetLog& net_log
#ifdef ANDROID
            , bool valid_uid, int calling_uid
#endif
           )
        : internal::ClientSocketPoolBaseHelper::Request(
              handle, callback, priority, ignore_limits,
              flags, net_log
#ifdef ANDROID
              , valid_uid, calling_uid
#endif
              ),
          params_(params) {}

    const scoped_refptr<SocketParams>& params() const { return params_; }

   private:
    const scoped_refptr<SocketParams> params_;
  };

  class ConnectJobFactory {
   public:
    ConnectJobFactory() {}
    virtual ~ConnectJobFactory() {}

    virtual ConnectJob* NewConnectJob(
        const std::string& group_name,
        const Request& request,
        ConnectJob::Delegate* delegate) const = 0;

    virtual base::TimeDelta ConnectionTimeout() const = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(ConnectJobFactory);
  };

  
  
  
  
  
  
  ClientSocketPoolBase(
      int max_sockets,
      int max_sockets_per_group,
      ClientSocketPoolHistograms* histograms,
      base::TimeDelta unused_idle_socket_timeout,
      base::TimeDelta used_idle_socket_timeout,
      ConnectJobFactory* connect_job_factory,
      HttpNetworkSession *network_session)
      : histograms_(histograms),
        helper_(max_sockets, max_sockets_per_group,
                unused_idle_socket_timeout, used_idle_socket_timeout,
                new ConnectJobFactoryAdaptor(connect_job_factory),
                network_session) {}

  virtual ~ClientSocketPoolBase() {}

  

  
  
  int RequestSocket(const std::string& group_name,
                    const scoped_refptr<SocketParams>& params,
                    RequestPriority priority,
                    ClientSocketHandle* handle,
                    CompletionCallback* callback,
                    const BoundNetLog& net_log) {
#ifdef ANDROID
    uid_t calling_uid = 0;
    bool valid_uid = params->getUID(&calling_uid);
#endif
    Request* request =
        new Request(handle, callback, priority,
                    internal::ClientSocketPoolBaseHelper::NORMAL,
                    params->ignore_limits(),
                    params, net_log
#ifdef ANDROID
                    , valid_uid, calling_uid
#endif
                    );
    return helper_.RequestSocket(group_name, request);
  }

  
  
  
  void RequestSockets(const std::string& group_name,
                      const scoped_refptr<SocketParams>& params,
                      int num_sockets,
                      const BoundNetLog& net_log) {
#ifdef ANDROID
    uid_t calling_uid = 0;
    bool valid_uid = params->getUID(&calling_uid);
#endif
    const Request request(NULL ,
                          NULL ,
                          LOWEST,
                          internal::ClientSocketPoolBaseHelper::NO_IDLE_SOCKETS,
                          params->ignore_limits(),
                          params,
                          net_log
#ifdef ANDROID
                          , valid_uid, calling_uid
#endif
                          );
    helper_.RequestSockets(group_name, request, num_sockets);
  }

  void CancelRequest(const std::string& group_name,
                     ClientSocketHandle* handle) {
    return helper_.CancelRequest(group_name, handle);
  }

  void ReleaseSocket(const std::string& group_name, ClientSocket* socket,
                     int id) {
    return helper_.ReleaseSocket(group_name, socket, id);
  }

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

  DictionaryValue* GetInfoAsValue(const std::string& name,
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

  void Flush() { helper_.Flush(); }

 private:
  
  
  
  
  
  class ConnectJobFactoryAdaptor
      : public internal::ClientSocketPoolBaseHelper::ConnectJobFactory {
   public:
    typedef typename ClientSocketPoolBase<SocketParams>::ConnectJobFactory
        ConnectJobFactory;

    explicit ConnectJobFactoryAdaptor(ConnectJobFactory* connect_job_factory)
        : connect_job_factory_(connect_job_factory) {}
    virtual ~ConnectJobFactoryAdaptor() {}

    virtual ConnectJob* NewConnectJob(
        const std::string& group_name,
        const internal::ClientSocketPoolBaseHelper::Request& request,
        ConnectJob::Delegate* delegate) const {
      const Request* casted_request = static_cast<const Request*>(&request);
      return connect_job_factory_->NewConnectJob(
          group_name, *casted_request, delegate);
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
