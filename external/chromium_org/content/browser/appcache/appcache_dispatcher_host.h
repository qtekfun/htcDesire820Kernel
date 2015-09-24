// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_APPCACHE_APPCACHE_DISPATCHER_HOST_H_
#define CONTENT_BROWSER_APPCACHE_APPCACHE_DISPATCHER_HOST_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "content/browser/appcache/appcache_frontend_proxy.h"
#include "content/public/browser/browser_message_filter.h"
#include "webkit/browser/appcache/appcache_backend_impl.h"

namespace content {
class ChromeAppCacheService;

class AppCacheDispatcherHost : public BrowserMessageFilter {
 public:
  AppCacheDispatcherHost(ChromeAppCacheService* appcache_service,
                         int process_id);

  
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 protected:
  virtual ~AppCacheDispatcherHost();

  
  virtual void BadMessageReceived() OVERRIDE;

 private:
  
  void OnRegisterHost(int host_id);
  void OnUnregisterHost(int host_id);
  void OnSetSpawningHostId(int host_id, int spawning_host_id);
  void OnSelectCache(int host_id, const GURL& document_url,
                     int64 cache_document_was_loaded_from,
                     const GURL& opt_manifest_url);
  void OnSelectCacheForWorker(int host_id, int parent_process_id,
                              int parent_host_id);
  void OnSelectCacheForSharedWorker(int host_id, int64 appcache_id);
  void OnMarkAsForeignEntry(int host_id, const GURL& document_url,
                            int64 cache_document_was_loaded_from);
  void OnGetStatus(int host_id, IPC::Message* reply_msg);
  void OnStartUpdate(int host_id, IPC::Message* reply_msg);
  void OnSwapCache(int host_id, IPC::Message* reply_msg);
  void OnGetResourceList(
      int host_id,
      std::vector<appcache::AppCacheResourceInfo>* resource_infos);
  void GetStatusCallback(appcache::Status status, void* param);
  void StartUpdateCallback(bool result, void* param);
  void SwapCacheCallback(bool result, void* param);


  scoped_refptr<ChromeAppCacheService> appcache_service_;
  AppCacheFrontendProxy frontend_proxy_;
  appcache::AppCacheBackendImpl backend_impl_;

  appcache::GetStatusCallback get_status_callback_;
  appcache::StartUpdateCallback start_update_callback_;
  appcache::SwapCacheCallback swap_cache_callback_;
  scoped_ptr<IPC::Message> pending_reply_msg_;

  
  int process_id_;

  DISALLOW_COPY_AND_ASSIGN(AppCacheDispatcherHost);
};

}  

#endif  
