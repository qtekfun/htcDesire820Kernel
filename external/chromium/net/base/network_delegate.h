// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_DELEGATE_H_
#define NET_BASE_NETWORK_DELEGATE_H_
#pragma once

#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"

class GURL;

namespace net {

class HttpRequestHeaders;
class URLRequest;
class URLRequestJob;

class NetworkDelegate : public base::NonThreadSafe {
 public:
  virtual ~NetworkDelegate() {}

  
  
  
  
  int NotifyBeforeURLRequest(URLRequest* request,
                             CompletionCallback* callback,
                             GURL* new_url);
  int NotifyBeforeSendHeaders(uint64 request_id,
                              CompletionCallback* callback,
                              HttpRequestHeaders* headers);
  void NotifyResponseStarted(URLRequest* request);
  void NotifyReadCompleted(URLRequest* request, int bytes_read);
  void NotifyURLRequestDestroyed(URLRequest* request);

  
  
  
  
  
  
  URLRequestJob* MaybeCreateURLRequestJob(URLRequest* request);

 private:
  
  
  

  
  
  
  
  
  virtual int OnBeforeURLRequest(URLRequest* request,
                                 CompletionCallback* callback,
                                 GURL* new_url) = 0;

  
  
  
  
  virtual int OnBeforeSendHeaders(uint64 request_id,
                                  CompletionCallback* callback,
                                  HttpRequestHeaders* headers) = 0;

  
  virtual void OnResponseStarted(URLRequest* request) = 0;

  
  virtual void OnReadCompleted(URLRequest* request, int bytes_read) = 0;

  
  
  
  virtual void OnURLRequestDestroyed(URLRequest* request) = 0;

  
  
  virtual URLRequestJob* OnMaybeCreateURLRequestJob(URLRequest* request) = 0;

};

}  

#endif  
