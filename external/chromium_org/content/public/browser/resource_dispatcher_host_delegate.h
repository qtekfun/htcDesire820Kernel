// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_DISPATCHER_HOST_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_DISPATCHER_HOST_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "webkit/common/resource_type.h"

class GURL;
template <class T> class ScopedVector;

namespace appcache {
class AppCacheService;
}

namespace content {
class ResourceContext;
class ResourceThrottle;
class StreamHandle;
struct Referrer;
struct ResourceResponse;
}

namespace IPC {
class Sender;
}

namespace net {
class AuthChallengeInfo;
class SSLCertRequestInfo;
class URLRequest;
}

namespace content {

class ResourceDispatcherHostLoginDelegate;

class CONTENT_EXPORT ResourceDispatcherHostDelegate {
 public:
  
  virtual bool ShouldBeginRequest(
      int child_id,
      int route_id,
      const std::string& method,
      const GURL& url,
      ResourceType::Type resource_type,
      ResourceContext* resource_context);

  
  
  virtual void RequestBeginning(
      net::URLRequest* request,
      ResourceContext* resource_context,
      appcache::AppCacheService* appcache_service,
      ResourceType::Type resource_type,
      int child_id,
      int route_id,
      ScopedVector<ResourceThrottle>* throttles);

  
  virtual void WillTransferRequestToNewProcess(
      int old_child_id,
      int old_route_id,
      int old_request_id,
      int new_child_id,
      int new_route_id,
      int new_request_id);

  
  
  virtual void DownloadStarting(
      net::URLRequest* request,
      ResourceContext* resource_context,
      int child_id,
      int route_id,
      int request_id,
      bool is_content_initiated,
      bool must_download,
      ScopedVector<ResourceThrottle>* throttles);

  
  
  virtual bool AcceptSSLClientCertificateRequest(
      net::URLRequest* request,
      net::SSLCertRequestInfo* cert_request_info);

  
  
  
  
  virtual bool AcceptAuthRequest(net::URLRequest* request,
                                 net::AuthChallengeInfo* auth_info);

  
  
  virtual ResourceDispatcherHostLoginDelegate* CreateLoginDelegate(
      net::AuthChallengeInfo* auth_info, net::URLRequest* request);

  
  
  
  virtual bool HandleExternalProtocol(const GURL& url,
                                      int child_id,
                                      int route_id);

  
  
  virtual bool ShouldForceDownloadResource(
      const GURL& url, const std::string& mime_type);

  
  
  
  
  
  
  
  
  virtual bool ShouldInterceptResourceAsStream(
      content::ResourceContext* resource_context,
      const GURL& url,
      const std::string& mime_type,
      GURL* origin,
      std::string* target_id);

  
  
  
  
  virtual void OnStreamCreated(
      content::ResourceContext* resource_context,
      int render_process_id,
      int render_view_id,
      const std::string& target_id,
      scoped_ptr<StreamHandle> stream,
      int64 expected_content_size);

  
  virtual void OnResponseStarted(
      net::URLRequest* request,
      ResourceContext* resource_context,
      ResourceResponse* response,
      IPC::Sender* sender);

  
  virtual void OnRequestRedirected(
      const GURL& redirect_url,
      net::URLRequest* request,
      ResourceContext* resource_context,
      ResourceResponse* response);

 protected:
  ResourceDispatcherHostDelegate();
  virtual ~ResourceDispatcherHostDelegate();
};

}  

#endif  
