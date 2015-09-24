// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_WORKER_WORKER_WEBAPPLICATIONCACHEHOST_IMPL_H_
#define CHROME_WORKER_WORKER_WEBAPPLICATIONCACHEHOST_IMPL_H_

#include "content/child/appcache/web_application_cache_host_impl.h"

namespace content {

struct WorkerAppCacheInitInfo {
  int parent_process_id;
  int64 main_resource_appcache_id;  

  WorkerAppCacheInitInfo()
      : parent_process_id(0),
        main_resource_appcache_id(0) {
  }
  WorkerAppCacheInitInfo(
      int process_id, int64 cache_id)
      : parent_process_id(process_id),
        main_resource_appcache_id(cache_id) {
  }
};

class WorkerWebApplicationCacheHostImpl : public WebApplicationCacheHostImpl {
 public:
  WorkerWebApplicationCacheHostImpl(
      const WorkerAppCacheInitInfo& init_info,
      blink::WebApplicationCacheHostClient* client);

  
  
  
  virtual void willStartMainResourceRequest(
      blink::WebURLRequest&, const blink::WebFrame*);
  virtual void didReceiveResponseForMainResource(
      const blink::WebURLResponse&);
  virtual void didReceiveDataForMainResource(const char* data, int len);
  virtual void didFinishLoadingMainResource(bool success);

  
  
  
  virtual void selectCacheWithoutManifest();
  virtual bool selectCacheWithManifest(const blink::WebURL& manifestURL);
};

}  

#endif  
