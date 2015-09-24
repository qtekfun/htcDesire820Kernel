// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_BACKEND_IMPL_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_BACKEND_IMPL_H_

#include "base/containers/hash_tables.h"
#include "webkit/browser/appcache/appcache_host.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace appcache {

class AppCacheService;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheBackendImpl {
 public:
  AppCacheBackendImpl();
  ~AppCacheBackendImpl();

  void Initialize(AppCacheService* service,
                  AppCacheFrontend* frontend,
                  int process_id);

  int process_id() const { return process_id_; }

  
  
  
  bool RegisterHost(int host_id);
  bool UnregisterHost(int host_id);
  bool SetSpawningHostId(int host_id, int spawning_host_id);
  bool SelectCache(int host_id,
                   const GURL& document_url,
                   const int64 cache_document_was_loaded_from,
                   const GURL& manifest_url);
  void GetResourceList(
      int host_id, std::vector<AppCacheResourceInfo>* resource_infos);
  bool SelectCacheForWorker(int host_id, int parent_process_id,
                            int parent_host_id);
  bool SelectCacheForSharedWorker(int host_id, int64 appcache_id);
  bool MarkAsForeignEntry(int host_id, const GURL& document_url,
                          int64 cache_document_was_loaded_from);
  bool GetStatusWithCallback(int host_id, const GetStatusCallback& callback,
                             void* callback_param);
  bool StartUpdateWithCallback(int host_id, const StartUpdateCallback& callback,
                               void* callback_param);
  bool SwapCacheWithCallback(int host_id, const SwapCacheCallback& callback,
                             void* callback_param);

  
  AppCacheHost* GetHost(int host_id) {
    HostMap::iterator it = hosts_.find(host_id);
    return (it != hosts_.end()) ? (it->second) : NULL;
  }

  typedef base::hash_map<int, AppCacheHost*> HostMap;
  const HostMap& hosts() { return hosts_; }

  
  
  
  scoped_ptr<AppCacheHost> TransferHostOut(int host_id);
  void TransferHostIn(int new_host_id, scoped_ptr<AppCacheHost> host);

 private:
  AppCacheService* service_;
  AppCacheFrontend* frontend_;
  int process_id_;
  HostMap hosts_;
};

}  

#endif  
