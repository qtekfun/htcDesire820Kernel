// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_HTTP_HANDLER_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_HTTP_HANDLER_DELEGATE_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "net/socket/stream_listen_socket.h"

class GURL;

namespace content {

class DevToolsTarget;

class DevToolsHttpHandlerDelegate {
 public:
  virtual ~DevToolsHttpHandlerDelegate() {}

  
  
  virtual std::string GetDiscoveryPageHTML() = 0;

  
  virtual bool BundlesFrontendResources() = 0;

  
  virtual base::FilePath GetDebugFrontendDir() = 0;

  
  
  virtual std::string GetPageThumbnailData(const GURL& url) = 0;

  
  virtual scoped_ptr<DevToolsTarget> CreateNewTarget(const GURL& url) = 0;

  typedef std::vector<DevToolsTarget*> TargetList;
  typedef base::Callback<void(const TargetList&)> TargetCallback;

  
  
  virtual void EnumerateTargets(TargetCallback callback) = 0;

  
  
  virtual scoped_ptr<net::StreamListenSocket> CreateSocketForTethering(
      net::StreamListenSocket::Delegate* delegate,
      std::string* name) = 0;
};

}  

#endif  
