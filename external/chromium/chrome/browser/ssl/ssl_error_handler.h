// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_ERROR_HANDLER_H_
#define CHROME_BROWSER_SSL_SSL_ERROR_HANDLER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/ssl/ssl_manager.h"
#include "content/browser/renderer_host/global_request_id.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/resource_type.h"

class ResourceDispatcherHost;
class SSLCertErrorHandler;
class TabContents;

namespace net {
class URLRequest;
}  

class SSLErrorHandler : public base::RefCountedThreadSafe<SSLErrorHandler> {
 public:
  virtual SSLCertErrorHandler* AsSSLCertErrorHandler();

  
  
  
  
  void Dispatch();

  
  const GURL& request_url() const { return request_url_; }

  
  ResourceType::Type resource_type() const { return resource_type_; }

  
  
  TabContents* GetTabContents();

  
  
  void CancelRequest();

  
  
  
  
  void ContinueRequest();

  
  
  
  
  
  void DenyRequest();

  
  
  
  
  void TakeNoAction();

 protected:
  friend class base::RefCountedThreadSafe<SSLErrorHandler>;

  
  SSLErrorHandler(ResourceDispatcherHost* resource_dispatcher_host,
                  net::URLRequest* request,
                  ResourceType::Type resource_type);

  virtual ~SSLErrorHandler();

  
  virtual void OnDispatchFailed();

  
  virtual void OnDispatched();

  
  SSLManager* manager_;  

  
  
  GlobalRequestID request_id_;

  
  ResourceDispatcherHost* resource_dispatcher_host_;

 private:
  
  
  void CompleteCancelRequest(int error);

  
  
  
  void CompleteContinueRequest();

  
  
  void CompleteTakeNoAction();

  
  
  int render_process_host_id_;
  int tab_contents_id_;

  
  
  const GURL request_url_;

  
  
  
  const ResourceType::Type resource_type_;

  
  
  bool request_has_been_notified_;

  DISALLOW_COPY_AND_ASSIGN(SSLErrorHandler);
};

#endif  
