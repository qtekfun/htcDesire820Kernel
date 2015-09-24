// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_STORAGE_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_STORAGE_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "webkit/browser/appcache/appcache_working_set.h"
#include "webkit/browser/webkit_storage_browser_export.h"

class GURL;

namespace appcache {

class AppCache;
class AppCacheEntry;
class AppCacheGroup;
class AppCacheResponseReader;
class AppCacheResponseWriter;
class AppCacheService;
struct AppCacheInfoCollection;
struct HttpResponseInfoIOBuffer;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheStorage {
 public:
  typedef std::map<GURL, int64> UsageMap;

  class WEBKIT_STORAGE_BROWSER_EXPORT Delegate {
   public:
    
    virtual void OnAllInfo(AppCacheInfoCollection* collection) {}

    
    virtual void OnCacheLoaded(AppCache* cache, int64 cache_id) {}

    
    virtual void OnGroupLoaded(
        AppCacheGroup* group, const GURL& manifest_url) {}

    
    virtual void OnGroupAndNewestCacheStored(
        AppCacheGroup* group, AppCache* newest_cache, bool success,
        bool would_exceed_quota) {}

    
    virtual void OnGroupMadeObsolete(AppCacheGroup* group, bool success) {}

    
    virtual void OnResponseInfoLoaded(
        AppCacheResponseInfo* response_info, int64 response_id) {}

    
    
    
    
    
    
    virtual void OnMainResponseFound(
        const GURL& url, const AppCacheEntry& entry,
        const GURL& namespace_entry_url, const AppCacheEntry& fallback_entry,
        int64 cache_id, int64 group_id, const GURL& mainfest_url) {}

   protected:
    virtual ~Delegate() {}
  };

  explicit AppCacheStorage(AppCacheService* service);
  virtual ~AppCacheStorage();

  
  
  
  virtual void GetAllInfo(Delegate* delegate) = 0;

  
  
  
  
  
  virtual void LoadCache(int64 id, Delegate* delegate) = 0;

  
  
  
  
  
  virtual void LoadOrCreateGroup(
      const GURL& manifest_url, Delegate* delegate) = 0;

  
  
  
  
  
  virtual void LoadResponseInfo(
      const GURL& manifest_url, int64 group_id, int64 response_id,
      Delegate* delegate);

  
  
  
  
  
  
  
  
  
  virtual void StoreGroupAndNewestCache(
      AppCacheGroup* group, AppCache* newest_cache, Delegate* delegate) = 0;

  
  
  virtual void FindResponseForMainRequest(
      const GURL& url,
      const GURL& preferred_manifest_url,
      Delegate* delegate) = 0;

  
  
  virtual void FindResponseForSubRequest(
      AppCache* cache, const GURL& url,
      AppCacheEntry* found_entry, AppCacheEntry* found_fallback_entry,
      bool* found_network_namespace) = 0;

  
  
  
  
  virtual void MarkEntryAsForeign(const GURL& entry_url, int64 cache_id) = 0;

  
  
  
  virtual void MakeGroupObsolete(
      AppCacheGroup* group, Delegate* delegate) = 0;

  
  
  
  
  void CancelDelegateCallbacks(Delegate* delegate) {
    DelegateReference* delegate_reference = GetDelegateReference(delegate);
    if (delegate_reference)
      delegate_reference->CancelReference();
  }

  
  virtual AppCacheResponseReader* CreateResponseReader(
      const GURL& manifest_url, int64 group_id, int64 response_id) = 0;

  
  
  virtual AppCacheResponseWriter* CreateResponseWriter(
      const GURL& manifest_url, int64 group_id) = 0;

  
  
  
  virtual void DoomResponses(
      const GURL& manifest_url, const std::vector<int64>& response_ids) = 0;

  
  
  virtual void DeleteResponses(
      const GURL& manifest_url, const std::vector<int64>& response_ids) = 0;

  virtual void PurgeMemory() = 0;

  
  int64 NewCacheId() {
    return ++last_cache_id_;
  }
  int64 NewGroupId() {
    return ++last_group_id_;
  }

  
  AppCacheWorkingSet* working_set() { return &working_set_; }

  
  const UsageMap* usage_map() { return &usage_map_; }

  
  AppCacheService* service() { return service_; }

 protected:
  friend class AppCacheQuotaClientTest;
  friend class AppCacheResponseTest;
  friend class AppCacheStorageTest;

  
  #define FOR_EACH_DELEGATE(delegates, func_and_args)                \
    do {                                                             \
      for (DelegateReferenceVector::iterator it = delegates.begin(); \
           it != delegates.end(); ++it) {                            \
        if (it->get()->delegate)                                     \
          it->get()->delegate->func_and_args;                        \
      }                                                              \
    } while (0)

  
  
  struct DelegateReference : public base::RefCounted<DelegateReference> {
    Delegate* delegate;
    AppCacheStorage* storage;

    DelegateReference(Delegate* delegate, AppCacheStorage* storage);

    void CancelReference() {
      storage->delegate_references_.erase(delegate);
      storage = NULL;
      delegate = NULL;
    }

   private:
    friend class base::RefCounted<DelegateReference>;

    virtual ~DelegateReference();
  };
  typedef std::map<Delegate*, DelegateReference*> DelegateReferenceMap;
  typedef std::vector<scoped_refptr<DelegateReference> >
      DelegateReferenceVector;

  
  
  class ResponseInfoLoadTask {
   public:
    ResponseInfoLoadTask(const GURL& manifest_url, int64 group_id,
                         int64 response_id, AppCacheStorage* storage);
    ~ResponseInfoLoadTask();

    int64 response_id() const { return response_id_; }
    const GURL& manifest_url() const { return manifest_url_; }
    int64 group_id() const { return group_id_; }

    void AddDelegate(DelegateReference* delegate_reference) {
      delegates_.push_back(delegate_reference);
    }

    void StartIfNeeded();

   private:
    void OnReadComplete(int result);

    AppCacheStorage* storage_;
    GURL manifest_url_;
    int64 group_id_;
    int64 response_id_;
    scoped_ptr<AppCacheResponseReader> reader_;
    DelegateReferenceVector delegates_;
    scoped_refptr<HttpResponseInfoIOBuffer> info_buffer_;
  };

  typedef std::map<int64, ResponseInfoLoadTask*> PendingResponseInfoLoads;

  DelegateReference* GetDelegateReference(Delegate* delegate) {
    DelegateReferenceMap::iterator iter =
        delegate_references_.find(delegate);
    if (iter != delegate_references_.end())
      return iter->second;
    return NULL;
  }

  DelegateReference* GetOrCreateDelegateReference(Delegate* delegate) {
    DelegateReference* reference = GetDelegateReference(delegate);
    if (reference)
      return reference;
    return new DelegateReference(delegate, this);
  }

  ResponseInfoLoadTask* GetOrCreateResponseInfoLoadTask(
      const GURL& manifest_url, int64 group_id, int64 response_id) {
    PendingResponseInfoLoads::iterator iter =
        pending_info_loads_.find(response_id);
    if (iter != pending_info_loads_.end())
      return iter->second;
    return new ResponseInfoLoadTask(manifest_url, group_id, response_id, this);
  }

  
  int64 NewResponseId() {
    return ++last_response_id_;
  }

  
  void UpdateUsageMapAndNotify(const GURL& origin, int64 new_usage);
  void ClearUsageMapAndNotify();
  void NotifyStorageAccessed(const GURL& origin);

  
  int64 last_cache_id_;
  int64 last_group_id_;
  int64 last_response_id_;

  UsageMap usage_map_;  
  AppCacheWorkingSet working_set_;
  AppCacheService* service_;
  DelegateReferenceMap delegate_references_;
  PendingResponseInfoLoads pending_info_loads_;

  
  static const int64 kUnitializedId;

  FRIEND_TEST_ALL_PREFIXES(AppCacheStorageTest, DelegateReferences);
  FRIEND_TEST_ALL_PREFIXES(AppCacheStorageTest, UsageMap);

  DISALLOW_COPY_AND_ASSIGN(AppCacheStorage);
};

}  

#endif  
