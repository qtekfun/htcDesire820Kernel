// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SSL_SSL_ERROR_HANDLER_H_
#define CONTENT_BROWSER_SSL_SSL_ERROR_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/public/browser/global_request_id.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"

namespace net {
class SSLInfo;
class URLRequest;
}  

namespace content {

class ResourceDispatcherHostImpl;
class SSLCertErrorHandler;
class SSLManager;

class SSLErrorHandler : public base::RefCountedThreadSafe<SSLErrorHandler> {
 public:
  
  
  
  
  
  class CONTENT_EXPORT Delegate {
   public:
    
    
    virtual void CancelSSLRequest(const GlobalRequestID& id,
                                  int error,
                                  const net::SSLInfo* ssl_info) = 0;

    
    
    virtual void ContinueSSLRequest(const GlobalRequestID& id) = 0;

   protected:
    virtual ~Delegate() {}
  };

  virtual SSLCertErrorHandler* AsSSLCertErrorHandler();

  
  
  
  
  void Dispatch();

  
  const GURL& request_url() const { return request_url_; }

  
  ResourceType::Type resource_type() const { return resource_type_; }

  
  
  CONTENT_EXPORT void CancelRequest();

  
  
  
  
  void ContinueRequest();

  
  
  
  
  
  void DenyRequest();

  
  
  
  
  void TakeNoAction();

  int render_process_id() const { return render_process_id_; }
  int render_view_id() const { return render_view_id_; }

 protected:
  friend class base::RefCountedThreadSafe<SSLErrorHandler>;

  
  SSLErrorHandler(const base::WeakPtr<Delegate>& delegate,
                  const GlobalRequestID& id,
                  ResourceType::Type resource_type,
                  const GURL& url,
                  int render_process_id,
                  int render_view_id);

  virtual ~SSLErrorHandler();

  
  virtual void OnDispatchFailed();

  
  virtual void OnDispatched();

  
  SSLManager* manager_;  

  
  
  GlobalRequestID request_id_;

  
  base::WeakPtr<Delegate> delegate_;

 private:
  
  
  void CompleteCancelRequest(int error);

  
  
  
  void CompleteContinueRequest();

  
  
  void CompleteTakeNoAction();

  
  
  int render_process_id_;
  int render_view_id_;

  
  
  const GURL request_url_;

  
  
  
  const ResourceType::Type resource_type_;

  
  
  bool request_has_been_notified_;

  DISALLOW_COPY_AND_ASSIGN(SSLErrorHandler);
};

}  

#endif  
