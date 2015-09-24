// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SCRIPT_FETCHER_IMPL_H_
#define NET_PROXY_PROXY_SCRIPT_FETCHER_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "net/proxy/proxy_script_fetcher.h"
#include "net/url_request/url_request.h"

class GURL;

namespace net {

class URLRequestContext;

class NET_EXPORT ProxyScriptFetcherImpl : public ProxyScriptFetcher,
                                          public URLRequest::Delegate {
 public:
  
  
  
  
  
  
  explicit ProxyScriptFetcherImpl(URLRequestContext* url_request_context);

  virtual ~ProxyScriptFetcherImpl();

  
  base::TimeDelta SetTimeoutConstraint(base::TimeDelta timeout);
  size_t SetSizeConstraint(size_t size_bytes);

  void OnResponseCompleted(URLRequest* request);

  
  virtual int Fetch(const GURL& url, base::string16* text,
                    const net::CompletionCallback& callback) OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual URLRequestContext* GetRequestContext() const OVERRIDE;

  
  virtual void OnAuthRequired(URLRequest* request,
                              AuthChallengeInfo* auth_info) OVERRIDE;
  virtual void OnSSLCertificateError(URLRequest* request,
                                     const SSLInfo& ssl_info,
                                     bool is_hsts_ok) OVERRIDE;
  virtual void OnResponseStarted(URLRequest* request) OVERRIDE;
  virtual void OnReadCompleted(URLRequest* request, int num_bytes) OVERRIDE;

 private:
  enum { kBufSize = 4096 };

  
  void ReadBody(URLRequest* request);

  
  
  bool ConsumeBytesRead(URLRequest* request, int num_bytes);

  
  
  void FetchCompleted();

  
  void ResetCurRequestState();

  
  void OnTimeout(int id);

  
  
  base::WeakPtrFactory<ProxyScriptFetcherImpl> weak_factory_;

  
  URLRequestContext* const url_request_context_;

  
  scoped_refptr<IOBuffer> buf_;

  
  int next_id_;

  
  scoped_ptr<URLRequest> cur_request_;

  

  
  int cur_request_id_;

  
  net::CompletionCallback callback_;

  
  int result_code_;

  
  std::string bytes_read_so_far_;

  
  
  base::string16* result_text_;

  
  size_t max_response_bytes_;

  
  base::TimeDelta max_duration_;

  DISALLOW_COPY_AND_ASSIGN(ProxyScriptFetcherImpl);
};

}  

#endif  
