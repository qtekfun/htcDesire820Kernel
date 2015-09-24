// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_URL_DATA_SOURCE_H_
#define CONTENT_PUBLIC_BROWSER_URL_DATA_SOURCE_H_

#include <string>

#include "base/callback.h"
#include "content/common/content_export.h"

namespace base {
class MessageLoop;
class RefCountedMemory;
}

namespace net {
class URLRequest;
}

namespace content {
class BrowserContext;

class CONTENT_EXPORT URLDataSource {
 public:
  
  static void Add(BrowserContext* browser_context, URLDataSource* source);

  virtual ~URLDataSource() {}

  
  
  
  virtual std::string GetSource() const = 0;

  
  
  typedef base::Callback<void(base::RefCountedMemory*)> GotDataCallback;

  
  
  
  
  virtual void StartDataRequest(const std::string& path,
                                int render_process_id,
                                int render_view_id,
                                const GotDataCallback& callback) = 0;

  
  
  virtual std::string GetMimeType(const std::string& path) const = 0;

  

  
  
  
  
  
  
  
  
  virtual base::MessageLoop* MessageLoopForRequestPath(
      const std::string& path) const;

  
  
  
  
  
  
  virtual bool ShouldReplaceExistingSource() const;

  
  virtual bool AllowCaching() const;

  
  
  
  
  
  
  
  virtual bool ShouldAddContentSecurityPolicy() const;

  
  

  
  virtual std::string GetContentSecurityPolicyObjectSrc() const;
  
  virtual std::string GetContentSecurityPolicyFrameSrc() const;

  
  
  virtual bool ShouldDenyXFrameOptions() const;

  
  
  
  
  
  virtual bool ShouldServiceRequest(const net::URLRequest* request) const;

  
  
  
  
  
  virtual bool ShouldServeMimeTypeAsContentTypeHeader() const;

  
  
  
  virtual void WillServiceRequest(
      const net::URLRequest* request,
      std::string* path) const {}
};

}  

#endif  
