// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_HOST_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_HOST_H_

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "url/gurl.h"
#include "webkit/browser/appcache/appcache_group.h"
#include "webkit/browser/appcache/appcache_service.h"
#include "webkit/browser/appcache/appcache_storage.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/appcache/appcache_interfaces.h"
#include "webkit/common/resource_type.h"

namespace net {
class URLRequest;
}  

namespace appcache {

class AppCache;
class AppCacheFrontend;
class AppCacheRequestHandler;

typedef base::Callback<void(Status, void*)> GetStatusCallback;
typedef base::Callback<void(bool, void*)> StartUpdateCallback;
typedef base::Callback<void(bool, void*)> SwapCacheCallback;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheHost
    : public AppCacheStorage::Delegate,
      public AppCacheGroup::UpdateObserver,
      public AppCacheService::Observer {
 public:

  class WEBKIT_STORAGE_BROWSER_EXPORT Observer {
   public:
    
    virtual void OnCacheSelectionComplete(AppCacheHost* host) = 0;

    
    virtual void OnDestructionImminent(AppCacheHost* host) = 0;

    virtual ~Observer() {}
  };

  AppCacheHost(int host_id, AppCacheFrontend* frontend,
               AppCacheService* service);
  virtual ~AppCacheHost();

  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  void SelectCache(const GURL& document_url,
                   const int64 cache_document_was_loaded_from,
                   const GURL& manifest_url);
  void SelectCacheForWorker(int parent_process_id,
                            int parent_host_id);
  void SelectCacheForSharedWorker(int64 appcache_id);
  void MarkAsForeignEntry(const GURL& document_url,
                          int64 cache_document_was_loaded_from);
  void GetStatusWithCallback(const GetStatusCallback& callback,
                             void* callback_param);
  void StartUpdateWithCallback(const StartUpdateCallback& callback,
                               void* callback_param);
  void SwapCacheWithCallback(const SwapCacheCallback& callback,
                             void* callback_param);

  
  
  
  void SetSpawningHostId(int spawning_process_id, int spawning_host_id);

  
  
  const AppCacheHost* GetSpawningHost() const;

  const GURL& preferred_manifest_url() const {
    return preferred_manifest_url_;
  }
  void set_preferred_manifest_url(const GURL& url) {
    preferred_manifest_url_ = url;
  }

  
  
  AppCacheRequestHandler* CreateRequestHandler(
      net::URLRequest* request, ResourceType::Type resource_type);

  
  void GetResourceList(std::vector<AppCacheResourceInfo>* resource_infos);

  
  
  
  
  
  
  void AssociateNoCache(const GURL& manifest_url);

  
  
  
  
  void AssociateIncompleteCache(AppCache* cache, const GURL& manifest_url);

  
  
  
  
  void AssociateCompleteCache(AppCache* cache);

  
  
  void SetSwappableCache(AppCacheGroup* group);

  
  void LoadMainResourceCache(int64 cache_id);

  
  
  void NotifyMainResourceIsNamespaceEntry(const GURL& namespace_entry_url);

  
  
  void NotifyMainResourceBlocked(const GURL& manifest_url);

  
  
  const GURL& pending_master_entry_url() const {
    return new_master_entry_url_;
  }

  int host_id() const { return host_id_; }
  AppCacheService* service() const { return service_; }
  AppCacheStorage* storage() const { return storage_; }
  AppCacheFrontend* frontend() const { return frontend_; }
  AppCache* associated_cache() const { return associated_cache_.get(); }

  bool is_selection_pending() const {
    return pending_selected_cache_id_ != kNoCacheId ||
           !pending_selected_manifest_url_.is_empty();
  }

  const GURL& first_party_url() const { return first_party_url_; }

  
  void PrepareForTransfer();
  void CompleteTransfer(int host_id, AppCacheFrontend* frontend);

 private:
  Status GetStatus();
  void LoadSelectedCache(int64 cache_id);
  void LoadOrCreateGroup(const GURL& manifest_url);

  
  void AssociateCacheHelper(AppCache* cache, const GURL& manifest_url);

  
  virtual void OnCacheLoaded(AppCache* cache, int64 cache_id) OVERRIDE;
  virtual void OnGroupLoaded(AppCacheGroup* group,
                             const GURL& manifest_url) OVERRIDE;
  
  virtual void OnServiceReinitialized(
      AppCacheStorageReference* old_storage_ref) OVERRIDE;

  void FinishCacheSelection(AppCache* cache, AppCacheGroup* group);
  void DoPendingGetStatus();
  void DoPendingStartUpdate();
  void DoPendingSwapCache();

  void ObserveGroupBeingUpdated(AppCacheGroup* group);

  
  virtual void OnUpdateComplete(AppCacheGroup* group) OVERRIDE;

  
  bool is_for_dedicated_worker() const {
    return parent_host_id_ != kNoHostId;
  }

  
  
  AppCacheHost* GetParentAppCacheHost() const;

  
  int host_id_;

  
  
  
  int spawning_host_id_;
  int spawning_process_id_;
  GURL preferred_manifest_url_;

  
  
  
  
  
  int parent_host_id_;
  int parent_process_id_;

  
  
  
  scoped_refptr<AppCacheStorageReference> disabled_storage_reference_;

  
  scoped_refptr<AppCache> associated_cache_;

  
  
  
  
  scoped_refptr<AppCache> swappable_cache_;

  
  scoped_refptr<AppCacheGroup> group_being_updated_;

  
  
  
  scoped_refptr<AppCache> newest_cache_of_group_being_updated_;

  
  
  scoped_refptr<AppCache> main_resource_cache_;
  int64 pending_main_resource_cache_id_;

  
  
  
  int64 pending_selected_cache_id_;
  GURL pending_selected_manifest_url_;

  
  GURL new_master_entry_url_;

  
  AppCacheFrontend* frontend_;

  
  AppCacheService* service_;

  
  
  
  
  
  
  
  AppCacheStorage* storage_;

  
  
  
  
  GetStatusCallback pending_get_status_callback_;
  StartUpdateCallback pending_start_update_callback_;
  SwapCacheCallback pending_swap_cache_callback_;
  void* pending_callback_param_;

  
  
  bool main_resource_was_namespace_entry_;
  GURL namespace_entry_url_;

  
  bool main_resource_blocked_;
  GURL blocked_manifest_url_;

  
  
  bool associated_cache_info_pending_;

  
  ObserverList<Observer> observers_;

  
  GURL origin_in_use_;

  
  GURL first_party_url_;

  friend class AppCacheStorageImplTest;
  friend class AppCacheRequestHandlerTest;
  friend class AppCacheUpdateJobTest;
  FRIEND_TEST_ALL_PREFIXES(AppCacheTest, CleanupUnusedCache);
  FRIEND_TEST_ALL_PREFIXES(AppCacheGroupTest, CleanupUnusedGroup);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, SelectNoCache);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, ForeignEntry);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, FailedCacheLoad);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, FailedGroupLoad);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, SetSwappableCache);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, ForDedicatedWorker);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, SelectCacheAllowed);
  FRIEND_TEST_ALL_PREFIXES(AppCacheHostTest, SelectCacheBlocked);
  FRIEND_TEST_ALL_PREFIXES(AppCacheGroupTest, QueueUpdate);

  DISALLOW_COPY_AND_ASSIGN(AppCacheHost);
};

}  

#endif  
