// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_AW_CONTENTS_IO_THREAD_CLIENT_H_
#define ANDROID_WEBVIEW_BROWSER_AW_CONTENTS_IO_THREAD_CLIENT_H_

#include <string>

#include "base/memory/scoped_ptr.h"

class GURL;

namespace net {
class URLRequest;
}

namespace android_webview {

class InterceptedRequestData;

class AwContentsIoThreadClient {
 public:
  
  enum CacheMode {
    LOAD_DEFAULT = -1,
    LOAD_NORMAL = 0,
    LOAD_CACHE_ELSE_NETWORK = 1,
    LOAD_NO_CACHE = 2,
    LOAD_CACHE_ONLY = 3,
  };

  virtual ~AwContentsIoThreadClient() {}

  
  
  virtual bool PendingAssociation() const = 0;

  
  
  virtual CacheMode GetCacheMode() const = 0;

  
  
  
  
  static scoped_ptr<AwContentsIoThreadClient> FromID(int render_process_id,
                                                     int render_view_id);

  
  virtual scoped_ptr<InterceptedRequestData> ShouldInterceptRequest(
      const GURL& location,
      const net::URLRequest* request) = 0;

  
  
  virtual bool ShouldBlockContentUrls() const = 0;

  
  
  virtual bool ShouldBlockFileUrls() const = 0;

  
  
  virtual bool ShouldBlockNetworkLoads() const = 0;

  
  
  
  virtual void NewDownload(const GURL& url,
                           const std::string& user_agent,
                           const std::string& content_disposition,
                           const std::string& mime_type,
                           int64 content_length) = 0;

  
  
  
  virtual void NewLoginRequest(const std::string& realm,
                               const std::string& account,
                               const std::string& args) = 0;
};

} 

#endif  
