// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_URL_REQUEST_JOB_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_URL_REQUEST_JOB_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "net/http/http_byte_range.h"
#include "net/url_request/url_request_job.h"
#include "webkit/browser/appcache/appcache_entry.h"
#include "webkit/browser/appcache/appcache_executable_handler.h"
#include "webkit/browser/appcache/appcache_response.h"
#include "webkit/browser/appcache/appcache_storage.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace net {
class GrowableIOBuffer;
};

namespace appcache {

class AppCacheHost;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheURLRequestJob
    : public net::URLRequestJob,
      public AppCacheStorage::Delegate {
 public:
  AppCacheURLRequestJob(net::URLRequest* request,
                        net::NetworkDelegate* network_delegate,
                        AppCacheStorage* storage,
                        AppCacheHost* host);

  
  
  
  void DeliverAppCachedResponse(const GURL& manifest_url, int64 group_id,
                                int64 cache_id, const AppCacheEntry& entry,
                                bool is_fallback);
  void DeliverNetworkResponse();
  void DeliverErrorResponse();

  bool is_waiting() const {
    return delivery_type_ == AWAITING_DELIVERY_ORDERS;
  }

  bool is_delivering_appcache_response() const {
    return delivery_type_ == APPCACHED_DELIVERY;
  }

  bool is_delivering_network_response() const {
    return delivery_type_ == NETWORK_DELIVERY;
  }

  bool is_delivering_error_response() const {
    return delivery_type_ == ERROR_DELIVERY;
  }

  
  
  
  const GURL& manifest_url() const { return manifest_url_; }
  int64 group_id() const { return group_id_; }
  int64 cache_id() const { return cache_id_; }
  const AppCacheEntry& entry() const { return entry_; }

  
  
  virtual void Kill() OVERRIDE;

  
  bool has_been_started() const {
    return has_been_started_;
  }

  
  bool has_been_killed() const {
    return has_been_killed_;
  }

  
  bool cache_entry_not_found() const {
    return cache_entry_not_found_;
  }

 protected:
  virtual ~AppCacheURLRequestJob();

 private:
  friend class AppCacheRequestHandlerTest;
  friend class AppCacheURLRequestJobTest;

  enum DeliveryType {
    AWAITING_DELIVERY_ORDERS,
    APPCACHED_DELIVERY,
    NETWORK_DELIVERY,
    ERROR_DELIVERY
  };

  
  bool has_delivery_orders() const {
    return !is_waiting();
  }

  void MaybeBeginDelivery();
  void BeginDelivery();

  
  void BeginExecutableHandlerDelivery();
  void OnExecutableSourceLoaded(int result);
  void InvokeExecutableHandler(AppCacheExecutableHandler* handler);
  void OnExecutableResponseCallback(
      const AppCacheExecutableHandler::Response& response);
  void BeginErrorDelivery(const char* message);

  
  virtual void OnResponseInfoLoaded(
      AppCacheResponseInfo* response_info, int64 response_id) OVERRIDE;
  virtual void OnCacheLoaded(AppCache* cache, int64 cache_id) OVERRIDE;

  const net::HttpResponseInfo* http_info() const;
  bool is_range_request() const { return range_requested_.IsValid(); }
  void SetupRangeResponse();

  
  void OnReadComplete(int result);

  
  virtual void Start() OVERRIDE;
  virtual net::LoadState GetLoadState() const OVERRIDE;
  virtual bool GetCharset(std::string* charset) OVERRIDE;
  virtual void GetResponseInfo(net::HttpResponseInfo* info) OVERRIDE;
  virtual bool ReadRawData(net::IOBuffer* buf,
                           int buf_size,
                           int *bytes_read) OVERRIDE;

  
  
  virtual void SetExtraRequestHeaders(
      const net::HttpRequestHeaders& headers) OVERRIDE;

  
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual int GetResponseCode() const OVERRIDE;

  AppCacheHost* host_;
  AppCacheStorage* storage_;
  base::TimeTicks start_time_tick_;
  bool has_been_started_;
  bool has_been_killed_;
  DeliveryType delivery_type_;
  GURL manifest_url_;
  int64 group_id_;
  int64 cache_id_;
  AppCacheEntry entry_;
  bool is_fallback_;
  bool cache_entry_not_found_;
  scoped_refptr<AppCacheResponseInfo> info_;
  scoped_refptr<net::GrowableIOBuffer> handler_source_buffer_;
  scoped_ptr<AppCacheResponseReader> handler_source_reader_;
  net::HttpByteRange range_requested_;
  scoped_ptr<net::HttpResponseInfo> range_response_info_;
  scoped_ptr<AppCacheResponseReader> reader_;
  scoped_refptr<AppCache> cache_;
  scoped_refptr<AppCacheGroup> group_;
  base::WeakPtrFactory<AppCacheURLRequestJob> weak_factory_;
};

}  

#endif  
