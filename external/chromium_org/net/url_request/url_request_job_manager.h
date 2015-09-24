// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_JOB_MANAGER_H_
#define NET_URL_REQUEST_URL_REQUEST_JOB_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "net/url_request/url_request.h"

template <typename T> struct DefaultSingletonTraits;

namespace net {

class URLRequestJobManager {
 public:
  
  static URLRequestJobManager* GetInstance();

  
  
  
  URLRequestJob* CreateJob(URLRequest* request,
                           NetworkDelegate* network_delegate) const;

  
  
  URLRequestJob* MaybeInterceptRedirect(URLRequest* request,
                                        NetworkDelegate* network_delegate,
                                        const GURL& location) const;

  
  
  
  
  URLRequestJob* MaybeInterceptResponse(
      URLRequest* request, NetworkDelegate* network_delegate) const;

  
  
  
  bool SupportsScheme(const std::string& scheme) const;

  
  
  
  URLRequest::ProtocolFactory* RegisterProtocolFactory(
      const std::string& scheme, URLRequest::ProtocolFactory* factory);

  
  void RegisterRequestInterceptor(URLRequest::Interceptor* interceptor);
  void UnregisterRequestInterceptor(URLRequest::Interceptor* interceptor);

 private:
  typedef std::map<std::string, URLRequest::ProtocolFactory*> FactoryMap;
  typedef std::vector<URLRequest::Interceptor*> InterceptorList;
  friend struct DefaultSingletonTraits<URLRequestJobManager>;

  URLRequestJobManager();
  ~URLRequestJobManager();

  
  
  
  
  bool IsAllowedThread() const {
#if 0
    if (!allowed_thread_initialized_) {
      allowed_thread_ = base::PlatformThread::CurrentId();
      allowed_thread_initialized_ = true;
    }
    return allowed_thread_ == base::PlatformThread::CurrentId();
#else
    
    
    
    
    
    
    
    
    return true;
  }

  
  
  mutable base::PlatformThreadId allowed_thread_;
  mutable bool allowed_thread_initialized_;
#endif

  mutable base::Lock lock_;
  FactoryMap factories_;
  InterceptorList interceptors_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestJobManager);
};

}  

#endif  
