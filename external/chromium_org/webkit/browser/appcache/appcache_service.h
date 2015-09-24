// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_SERVICE_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_SERVICE_H_

#include <map>
#include <set>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "net/base/completion_callback.h"
#include "net/base/net_errors.h"
#include "webkit/browser/appcache/appcache_storage.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/appcache/appcache_interfaces.h"

namespace net {
class URLRequestContext;
}  

namespace base {
class FilePath;
class MessageLoopProxy;
}

namespace quota {
class QuotaManagerProxy;
class SpecialStoragePolicy;
}

namespace appcache {

class AppCacheBackendImpl;
class AppCacheExecutableHandlerFactory;
class AppCacheQuotaClient;
class AppCachePolicy;

struct WEBKIT_STORAGE_BROWSER_EXPORT AppCacheInfoCollection
    : public base::RefCountedThreadSafe<AppCacheInfoCollection> {
  AppCacheInfoCollection();

  std::map<GURL, AppCacheInfoVector> infos_by_origin;

 private:
  friend class base::RefCountedThreadSafe<AppCacheInfoCollection>;
  virtual ~AppCacheInfoCollection();
};

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheStorageReference :
    public base::RefCounted<AppCacheStorageReference> {
 public:
  AppCacheStorage* storage() const { return storage_.get(); }
 private:
  friend class AppCacheService;
  friend class base::RefCounted<AppCacheStorageReference>;
  AppCacheStorageReference(scoped_ptr<AppCacheStorage> storage);
  ~AppCacheStorageReference();

  scoped_ptr<AppCacheStorage> storage_;
};

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheService {
 public:

  class WEBKIT_STORAGE_BROWSER_EXPORT Observer {
   public:
    
    
    
    
    virtual void OnServiceReinitialized(
        AppCacheStorageReference* old_storage_ref) = 0;
    virtual ~Observer() {}
  };

  
  explicit AppCacheService(quota::QuotaManagerProxy* quota_manager_proxy);
  virtual ~AppCacheService();

  void Initialize(const base::FilePath& cache_directory,
                  base::MessageLoopProxy* db_thread,
                  base::MessageLoopProxy* cache_thread);

  void AddObserver(Observer* observer) {
    observers_.AddObserver(observer);
  }

  void RemoveObserver(Observer* observer) {
    observers_.RemoveObserver(observer);
  }

  
  
  void Reinitialize();

  
  void PurgeMemory() {
    if (storage_)
      storage_->PurgeMemory();
  }

  
  
  void CanHandleMainResourceOffline(const GURL& url,
                                    const GURL& first_party,
                                    const net::CompletionCallback& callback);

  
  
  
  
  void GetAllAppCacheInfo(AppCacheInfoCollection* collection,
                          const net::CompletionCallback& callback);

  
  
  
  
  
  void DeleteAppCacheGroup(const GURL& manifest_url,
                           const net::CompletionCallback& callback);

  
  
  
  virtual void DeleteAppCachesForOrigin(
      const GURL& origin, const net::CompletionCallback& callback);

  
  
  void CheckAppCacheResponse(const GURL& manifest_url, int64 cache_id,
                             int64 response_id);

  
  
  
  
  net::URLRequestContext* request_context() const { return request_context_; }
  void set_request_context(net::URLRequestContext* context) {
    request_context_ = context;
  }

  
  
  
  AppCachePolicy* appcache_policy() const { return appcache_policy_; }
  void set_appcache_policy(AppCachePolicy* policy) {
    appcache_policy_ = policy;
  }

  
  
  
  
  AppCacheExecutableHandlerFactory* handler_factory() const {
    return handler_factory_;
  }
  void set_handler_factory(
      AppCacheExecutableHandlerFactory* factory) {
    handler_factory_ = factory;
  }

  quota::SpecialStoragePolicy* special_storage_policy() const {
    return special_storage_policy_.get();
  }
  void set_special_storage_policy(quota::SpecialStoragePolicy* policy);

  quota::QuotaManagerProxy* quota_manager_proxy() const {
    return quota_manager_proxy_.get();
  }

  AppCacheQuotaClient* quota_client() const {
    return quota_client_;
  }

  
  
  void RegisterBackend(AppCacheBackendImpl* backend_impl);
  void UnregisterBackend(AppCacheBackendImpl* backend_impl);
  AppCacheBackendImpl* GetBackend(int id) const {
    BackendMap::const_iterator it = backends_.find(id);
    return (it != backends_.end()) ? it->second : NULL;
  }

  AppCacheStorage* storage() const { return storage_.get(); }

  
  void set_force_keep_session_state() { force_keep_session_state_ = true; }
  bool force_keep_session_state() const { return force_keep_session_state_; }

 protected:
  friend class AppCacheStorageImplTest;
  friend class AppCacheServiceTest;

  class AsyncHelper;
  class CanHandleOfflineHelper;
  class DeleteHelper;
  class DeleteOriginHelper;
  class GetInfoHelper;
  class CheckResponseHelper;

  typedef std::set<AsyncHelper*> PendingAsyncHelpers;
  typedef std::map<int, AppCacheBackendImpl*> BackendMap;

  base::FilePath cache_directory_;
  scoped_refptr<base::MessageLoopProxy> db_thread_;
  scoped_refptr<base::MessageLoopProxy> cache_thread_;
  AppCachePolicy* appcache_policy_;
  AppCacheQuotaClient* quota_client_;
  AppCacheExecutableHandlerFactory* handler_factory_;
  scoped_ptr<AppCacheStorage> storage_;
  scoped_refptr<quota::SpecialStoragePolicy> special_storage_policy_;
  scoped_refptr<quota::QuotaManagerProxy> quota_manager_proxy_;
  PendingAsyncHelpers pending_helpers_;
  BackendMap backends_;  
  
  net::URLRequestContext* request_context_;
  
  bool force_keep_session_state_;
  bool was_reinitialized_;
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(AppCacheService);
};

}  

#endif  
