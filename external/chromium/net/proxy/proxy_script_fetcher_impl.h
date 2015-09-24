// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SCRIPT_FETCHER_IMPL_H_
#define NET_PROXY_PROXY_SCRIPT_FETCHER_IMPL_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/time.h"
#include "net/proxy/proxy_script_fetcher.h"
#include "net/url_request/url_request.h"

class GURL;
class X509Certificate;

namespace net {

class URLRequestContext;

class ProxyScriptFetcherImpl : public ProxyScriptFetcher,
                               public URLRequest::Delegate {
 public:
  
  
  
  
  
  
  explicit ProxyScriptFetcherImpl(URLRequestContext* url_request_context);

  virtual ~ProxyScriptFetcherImpl();

  
  base::TimeDelta SetTimeoutConstraint(base::TimeDelta timeout);
  size_t SetSizeConstraint(size_t size_bytes);

  virtual void OnResponseCompleted(URLRequest* request);

  
  virtual int Fetch(const GURL& url, string16* text,
                    CompletionCallback* callback);
  virtual void Cancel();
  virtual URLRequestContext* GetRequestContext();

  
  virtual void OnAuthRequired(URLRequest* request,
                              AuthChallengeInfo* auth_info);
  virtual void OnSSLCertificateError(URLRequest* request, int cert_error,
                                     X509Certificate* cert);
  virtual void OnResponseStarted(URLRequest* request);
  virtual void OnReadCompleted(URLRequest* request, int num_bytes);

 private:
  enum { kBufSize = 4096 };

  
  void ReadBody(URLRequest* request);

  
  
  bool ConsumeBytesRead(URLRequest* request, int num_bytes);

  
  
  void FetchCompleted();

  
  void ResetCurRequestState();

  
  void OnTimeout(int id);

  
  
  ScopedRunnableMethodFactory<ProxyScriptFetcherImpl> task_factory_;

  
  URLRequestContext* url_request_context_;

  
  scoped_refptr<IOBuffer> buf_;

  
  int next_id_;

  
  scoped_ptr<URLRequest> cur_request_;

  

  
  int cur_request_id_;

  
  CompletionCallback* callback_;

  
  int result_code_;

  
  std::string bytes_read_so_far_;

  
  
  string16* result_text_;

  
  size_t max_response_bytes_;

  
  base::TimeDelta max_duration_;

  DISALLOW_COPY_AND_ASSIGN(ProxyScriptFetcherImpl);
};

}  

#endif  
