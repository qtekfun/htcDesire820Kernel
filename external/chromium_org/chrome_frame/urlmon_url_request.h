// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_URLMON_URL_REQUEST_H_
#define CHROME_FRAME_URLMON_URL_REQUEST_H_

#include <urlmon.h>
#include <atlbase.h>
#include <atlcom.h>
#include <map>
#include <string>

#include "chrome_frame/plugin_url_request.h"
#include "chrome_frame/urlmon_moniker.h"
#include "chrome_frame/utils.h"

namespace base {
class MessageLoop;
class Thread;
}

class UrlmonUrlRequest;

class UrlmonUrlRequestManager
    : public PluginUrlRequestManager,
      public PluginUrlRequestDelegate {
 public:
  
  struct PrivacyInfo {
   public:
    struct PrivacyEntry {
      PrivacyEntry() : flags(0) {}
      std::wstring policy_ref;
      int32 flags;
    };

    typedef std::map<std::wstring, PrivacyEntry> PrivacyRecords;

    PrivacyInfo() : privacy_impacted(false) {}

    bool privacy_impacted;
    PrivacyRecords privacy_records;
  };

  UrlmonUrlRequestManager();
  ~UrlmonUrlRequestManager();

  
  
  void SetInfoForUrl(const std::wstring& url,
                     IMoniker* moniker, LPBC bind_context);

  
  PrivacyInfo privacy_info() {
    return privacy_info_;
  }

  virtual void AddPrivacyDataForUrl(const std::string& url,
                                    const std::string& policy_ref,
                                    int32 flags);

  
  void put_notification_window(HWND window) {
    notification_window_ = window;
  }

  
  
  void set_privileged_mode(bool privileged_mode) {
    privileged_mode_ = privileged_mode;
  }

  void set_container(IUnknown* container) {
    container_ = container;
  }

 private:
  friend class base::MessageLoop;

  
  virtual PluginUrlRequestManager::ThreadSafeFlags GetThreadSafeFlags();
  virtual void StartRequest(int request_id,
                            const AutomationURLRequest& request_info);
  virtual void ReadRequest(int request_id, int bytes_to_read);
  virtual void EndRequest(int request_id);
  virtual void DownloadRequestInHost(int request_id);
  virtual void StopAll();

  
  virtual void OnResponseStarted(
      int request_id, const char* mime_type, const char* headers, int size,
      base::Time last_modified, const std::string& redirect_url,
      int redirect_status, const net::HostPortPair& socket_address,
      uint64 upload_size);
  virtual void OnReadComplete(int request_id, const std::string& data);
  virtual void OnResponseEnd(int request_id,
                             const net::URLRequestStatus& status);

  
  
  
  void BindTerminated(IMoniker* moniker, IBindCtx* bind_ctx,
                      IStream* post_data, const char* request_headers);

  
  void DownloadRequestInHostHelper(UrlmonUrlRequest* request);

  
  typedef std::map<int, scoped_refptr<UrlmonUrlRequest> > RequestMap;
  RequestMap request_map_;
  RequestMap background_request_map_;

  
  
  scoped_refptr<UrlmonUrlRequest> LookupRequest(int request_id,
                                                RequestMap* request_map);
  
  
  base::Lock background_resource_map_lock_;

  
  void StopAllRequestsHelper(RequestMap* request_map,
                             base::Lock* request_map_lock);
  
  void StartRequestHelper(int request_id,
                          const AutomationURLRequest& request_info,
                          RequestMap* request_map,
                          base::Lock* request_map_lock);

  scoped_refptr<UrlmonUrlRequest> pending_request_;
  scoped_ptr<base::Thread> background_thread_;

  bool stopping_;

  
  
  bool background_worker_thread_enabled_;

  PrivacyInfo privacy_info_;
  
  HWND notification_window_;
  
  bool privileged_mode_;
  
  
  IUnknown* container_;
};

#endif  
