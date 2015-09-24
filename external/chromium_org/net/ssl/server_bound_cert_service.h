// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SERVER_BOUND_CERT_SERVICE_H_
#define NET_SSL_SERVER_BOUND_CERT_SERVICE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/ssl/server_bound_cert_store.h"

namespace base {
class TaskRunner;
}

namespace net {

class ServerBoundCertServiceJob;
class ServerBoundCertServiceRequest;
class ServerBoundCertServiceWorker;


class NET_EXPORT ServerBoundCertService
    : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  class NET_EXPORT RequestHandle {
   public:
    RequestHandle();
    ~RequestHandle();

    
    
    void Cancel();

    bool is_active() const { return request_ != NULL; }

   private:
    friend class ServerBoundCertService;

    void RequestStarted(ServerBoundCertService* service,
                        ServerBoundCertServiceRequest* request,
                        const CompletionCallback& callback);

    void OnRequestComplete(int result);

    ServerBoundCertService* service_;
    ServerBoundCertServiceRequest* request_;
    CompletionCallback callback_;
  };

  
  
  
  static const char kEPKIPassword[];

  
  
  
  ServerBoundCertService(
      ServerBoundCertStore* server_bound_cert_store,
      const scoped_refptr<base::TaskRunner>& task_runner);

  ~ServerBoundCertService();

  
  
  
  static std::string GetDomainForHost(const std::string& host);

  
  
  
  
  bool IsSystemTimeValid() const { return is_system_time_valid_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int GetOrCreateDomainBoundCert(
      const std::string& host,
      std::string* private_key,
      std::string* cert,
      const CompletionCallback& callback,
      RequestHandle* out_req);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int GetDomainBoundCert(
      const std::string& host,
      std::string* private_key,
      std::string* cert,
      const CompletionCallback& callback,
      RequestHandle* out_req);

  
  ServerBoundCertStore* GetCertStore();

  
  int cert_count();
  uint64 requests() const { return requests_; }
  uint64 cert_store_hits() const { return cert_store_hits_; }
  uint64 inflight_joins() const { return inflight_joins_; }
  uint64 workers_created() const { return workers_created_; }

 private:
  
  
  
  void CancelRequest(ServerBoundCertServiceRequest* req);

  void GotServerBoundCert(int err,
                          const std::string& server_identifier,
                          base::Time expiration_time,
                          const std::string& key,
                          const std::string& cert);
  void GeneratedServerBoundCert(
      const std::string& server_identifier,
      int error,
      scoped_ptr<ServerBoundCertStore::ServerBoundCert> cert);
  void HandleResult(int error,
                    const std::string& server_identifier,
                    const std::string& private_key,
                    const std::string& cert);

  
  
  
  bool JoinToInFlightRequest(const base::TimeTicks& request_start,
                             const std::string& domain,
                             std::string* private_key,
                             std::string* cert,
                             bool create_if_missing,
                             const CompletionCallback& callback,
                             RequestHandle* out_req);

  
  
  
  
  int LookupDomainBoundCert(const base::TimeTicks& request_start,
                            const std::string& domain,
                            std::string* private_key,
                            std::string* cert,
                            bool create_if_missing,
                            const CompletionCallback& callback,
                            RequestHandle* out_req);

  scoped_ptr<ServerBoundCertStore> server_bound_cert_store_;
  scoped_refptr<base::TaskRunner> task_runner_;

  
  
  std::map<std::string, ServerBoundCertServiceJob*> inflight_;

  uint64 requests_;
  uint64 cert_store_hits_;
  uint64 inflight_joins_;
  uint64 workers_created_;

  bool is_system_time_valid_;

  base::WeakPtrFactory<ServerBoundCertService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ServerBoundCertService);
};

}  

#endif  
