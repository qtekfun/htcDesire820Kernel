// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_REQUEST_HANDLER_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_REQUEST_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/supports_user_data.h"
#include "webkit/browser/appcache/appcache_entry.h"
#include "webkit/browser/appcache/appcache_host.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/resource_type.h"

namespace net {
class NetworkDelegate;
class URLRequest;
class URLRequestJob;
}  

namespace appcache {

class AppCacheURLRequestJob;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheRequestHandler
    : public base::SupportsUserData::Data,
      public AppCacheHost::Observer,
      public AppCacheStorage::Delegate  {
 public:
  virtual ~AppCacheRequestHandler();

  
  AppCacheURLRequestJob* MaybeLoadResource(
      net::URLRequest* request, net::NetworkDelegate* network_delegate);
  AppCacheURLRequestJob* MaybeLoadFallbackForRedirect(
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate,
      const GURL& location);
  AppCacheURLRequestJob* MaybeLoadFallbackForResponse(
      net::URLRequest* request, net::NetworkDelegate* network_delegate);

  void GetExtraResponseInfo(int64* cache_id, GURL* manifest_url);

  
  void PrepareForCrossSiteTransfer(int old_process_id);
  void CompleteCrossSiteTransfer(int new_process_id, int new_host_id);

  static bool IsMainResourceType(ResourceType::Type type) {
    return ResourceType::IsFrame(type) ||
           ResourceType::IsSharedWorker(type);
  }

 private:
  friend class AppCacheHost;

  
  AppCacheRequestHandler(AppCacheHost* host, ResourceType::Type resource_type);

  
  virtual void OnDestructionImminent(AppCacheHost* host) OVERRIDE;

  
  
  void DeliverAppCachedResponse(const AppCacheEntry& entry, int64 cache_id,
                                int64 group_id, const GURL& manifest_url,
                                bool is_fallback,
                                const GURL& namespace_entry_url);
  void DeliverNetworkResponse();
  void DeliverErrorResponse();

  
  AppCacheStorage* storage() const;

  bool is_main_resource() const {
    return IsMainResourceType(resource_type_);
  }

  
  

  void MaybeLoadMainResource(net::URLRequest* request,
                             net::NetworkDelegate* network_delegate);

  
  virtual void OnMainResponseFound(
      const GURL& url, const AppCacheEntry& entry,
      const GURL& fallback_url, const AppCacheEntry& fallback_entry,
      int64 cache_id, int64 group_id, const GURL& mainfest_url) OVERRIDE;

  
  

  void MaybeLoadSubResource(net::URLRequest* request,
                            net::NetworkDelegate* network_delegate);
  void ContinueMaybeLoadSubResource();

  
  virtual void OnCacheSelectionComplete(AppCacheHost* host) OVERRIDE;

  

  
  AppCacheHost* host_;

  
  ResourceType::Type resource_type_;

  
  bool is_waiting_for_cache_selection_;

  
  
  int64 found_group_id_;
  int64 found_cache_id_;
  AppCacheEntry found_entry_;
  AppCacheEntry found_fallback_entry_;
  GURL found_namespace_entry_url_;
  GURL found_manifest_url_;
  bool found_network_namespace_;

  
  
  
  
  bool cache_entry_not_found_;

  
  bool maybe_load_resource_executed_;

  
  scoped_refptr<AppCacheURLRequestJob> job_;

  
  
  scoped_ptr<AppCacheHost> host_for_cross_site_transfer_;

  friend class AppCacheRequestHandlerTest;
  DISALLOW_COPY_AND_ASSIGN(AppCacheRequestHandler);
};

}  

#endif  
