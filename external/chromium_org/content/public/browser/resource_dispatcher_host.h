// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_DISPATCHER_HOST_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_DISPATCHER_HOST_H_

#include "base/callback_forward.h"
#include "net/base/net_errors.h"

namespace net {
class URLRequest;
}

namespace content {

class DownloadItem;
class ResourceContext;
class ResourceDispatcherHostDelegate;
struct DownloadSaveInfo;
struct Referrer;

class CONTENT_EXPORT ResourceDispatcherHost {
 public:
  typedef base::Callback<void(DownloadItem*, net::Error)>
    DownloadStartedCallback;

  
  static ResourceDispatcherHost* Get();

  
  
  virtual void SetDelegate(ResourceDispatcherHostDelegate* delegate) = 0;

  
  
  virtual void SetAllowCrossOriginAuthPrompt(bool value) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual net::Error BeginDownload(
      scoped_ptr<net::URLRequest> request,
      const Referrer& referrer,
      bool is_content_initiated,
      ResourceContext* context,
      int child_id,
      int route_id,
      bool prefer_cache,
      scoped_ptr<DownloadSaveInfo> save_info,
      uint32 download_id,
      const DownloadStartedCallback& started_callback) = 0;

  
  virtual void ClearLoginDelegateForRequest(net::URLRequest* request) = 0;

  
  
  
  virtual void BlockRequestsForRoute(int child_id, int route_id) = 0;

  
  virtual void ResumeBlockedRequestsForRoute(int child_id, int route_id) = 0;

 protected:
  virtual ~ResourceDispatcherHost() {}
};

}  

#endif  
