// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_HTTP_HANDLER_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_HTTP_HANDLER_H_

#include <string>

#include "content/common/content_export.h"

class GURL;

namespace net {
class StreamListenSocketFactory;
class URLRequestContextGetter;
}

namespace content {

class DevToolsHttpHandlerDelegate;

class DevToolsHttpHandler {
 public:
  
  CONTENT_EXPORT static bool IsSupportedProtocolVersion(
      const std::string& version);

  
  CONTENT_EXPORT static int GetFrontendResourceId(
      const std::string& name);

  
  CONTENT_EXPORT static DevToolsHttpHandler* Start(
      const net::StreamListenSocketFactory* socket_factory,
      const std::string& frontend_url,
      DevToolsHttpHandlerDelegate* delegate);

  
  
  virtual void Stop() = 0;

  
  virtual GURL GetFrontendURL() = 0;

 protected:
  virtual ~DevToolsHttpHandler() {}
};

}  

#endif  
