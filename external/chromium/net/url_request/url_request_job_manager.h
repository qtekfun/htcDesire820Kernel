// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_JOB_MANAGER_H_
#define NET_URL_REQUEST_URL_REQUEST_JOB_MANAGER_H_
#pragma once

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

  
  
  
  URLRequestJob* CreateJob(URLRequest* request) const;

  
  
  URLRequestJob* MaybeInterceptRedirect(URLRequest* request,
                                             const GURL& location) const;

  
  
  
  
  URLRequestJob* MaybeInterceptResponse(URLRequest* request) const;

  
  
  
  bool SupportsScheme(const std::string& scheme) const;

  
  
  
  URLRequest::ProtocolFactory* RegisterProtocolFactory(
      const std::string& scheme, URLRequest::ProtocolFactory* factory);

  
  void RegisterRequestInterceptor(URLRequest::Interceptor* interceptor);
  void UnregisterRequestInterceptor(URLRequest::Interceptor* interceptor);

  void set_enable_file_access(bool enable) { enable_file_access_ = enable; }
  bool enable_file_access() const { return enable_file_access_; }

 private:
  typedef std::map<std::string, URLRequest::ProtocolFactory*> FactoryMap;
  typedef std::vector<URLRequest::Interceptor*> InterceptorList;
  friend struct DefaultSingletonTraits<URLRequestJobManager>;

  URLRequestJobManager();
  ~URLRequestJobManager();

#ifndef NDEBUG
  
  
  
  
  bool IsAllowedThread() const {
#if 0
    if (!allowed_thread_initialized_) {
      allowed_thread_ = base::PlatformThread::CurrentId();
      allowed_thread_initialized_ = true;
    }
    return allowed_thread_ == base::PlatformThread::CurrentId();
#else
    
    
    
    
    
    
    
    
    return true;
#endif
  }

  
  
  mutable base::PlatformThreadId allowed_thread_;
  mutable bool allowed_thread_initialized_;
#endif

  mutable base::Lock lock_;
  FactoryMap factories_;
  InterceptorList interceptors_;
  bool enable_file_access_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestJobManager);
};

}  

#endif  
