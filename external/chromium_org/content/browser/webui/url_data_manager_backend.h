// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEBUI_URL_DATA_MANAGER_BACKEND_H_
#define CONTENT_BROWSER_WEBUI_URL_DATA_MANAGER_BACKEND_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/supports_user_data.h"
#include "content/browser/webui/url_data_manager.h"
#include "content/public/browser/url_data_source.h"
#include "net/url_request/url_request_job_factory.h"

class GURL;

namespace appcache {
class AppCacheService;
}

namespace base {
class RefCountedMemory;
}

namespace content {
class ChromeBlobStorageContext;
class ResourceContext;
class URLDataManagerBackend;
class URLDataSourceImpl;
class URLRequestChromeJob;

class URLDataManagerBackend : public base::SupportsUserData::Data {
 public:
  typedef int RequestID;

  URLDataManagerBackend();
  virtual ~URLDataManagerBackend();

  
  
  static net::URLRequestJobFactory::ProtocolHandler* CreateProtocolHandler(
      content::ResourceContext* resource_context,
      bool is_incognito,
      appcache::AppCacheService* appcache_service,
      ChromeBlobStorageContext* blob_storage_context);

  
  void AddDataSource(URLDataSourceImpl* source);

  
  void DataAvailable(RequestID request_id, base::RefCountedMemory* bytes);

  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     const std::string& scheme);

 private:
  friend class URLRequestChromeJob;

  typedef std::map<std::string,
      scoped_refptr<URLDataSourceImpl> > DataSourceMap;
  typedef std::map<RequestID, URLRequestChromeJob*> PendingRequestMap;

  
  
  bool StartRequest(const net::URLRequest* request, URLRequestChromeJob* job);

  
  
  
  static void CallStartRequest(scoped_refptr<URLDataSourceImpl> source,
                               const std::string& path,
                               int render_process_id,
                               int render_view_id,
                               int request_id);

  
  void RemoveRequest(URLRequestChromeJob* job);

  
  
  
  bool HasPendingJob(URLRequestChromeJob* job) const;

  
  DataSourceMap data_sources_;

  
  
  
  PendingRequestMap pending_requests_;

  
  RequestID next_request_id_;

  DISALLOW_COPY_AND_ASSIGN(URLDataManagerBackend);
};

net::URLRequestJobFactory::ProtocolHandler*
CreateDevToolsProtocolHandler(content::ResourceContext* resource_context,
                              bool is_incognito);

}  

#endif  
