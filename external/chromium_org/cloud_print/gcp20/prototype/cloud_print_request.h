// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_PRINT_REQUEST_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_PRINT_REQUEST_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"

class CloudPrintRequest : public net::URLFetcherDelegate,
                          public base::SupportsWeakPtr<CloudPrintRequest> {
 public:
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}

    
    
    virtual void OnFetchComplete(const std::string& response) = 0;

    
    
    virtual void OnFetchError(const std::string& server_api,
                              int server_code,
                              int server_http_code) = 0;

    
    
    virtual void OnFetchTimeoutReached() = 0;
  };

  virtual ~CloudPrintRequest();

  
  static scoped_ptr<CloudPrintRequest> CreateGet(const GURL& url,
                                                 Delegate* delegate);

  
  static scoped_ptr<CloudPrintRequest> CreatePost(const GURL& url,
                                                  const std::string& content,
                                                  const std::string& mimetype,
                                                  Delegate* delegate);

  
  void Run(const std::string& access_token,
           scoped_refptr<net::URLRequestContextGetter> context_getter);

  
  void AddHeader(const std::string& header);

 private:
  CloudPrintRequest(const GURL& url,
                    net::URLFetcher::RequestType method,
                    Delegate* delegate);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void OnRequestTimeout();

  scoped_ptr<net::URLFetcher> fetcher_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintRequest);
};

#endif  

