// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_DISTILLER_URL_FETCHER_H_
#define COMPONENTS_DOM_DISTILLER_CORE_DISTILLER_URL_FETCHER_H_

#include <string>

#include "base/callback.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "net/url_request/url_request_context_getter.h"

namespace dom_distiller {

class DistillerURLFetcher;

class DistillerURLFetcherFactory {
 public:
  explicit DistillerURLFetcherFactory(
      net::URLRequestContextGetter* context_getter);
  virtual ~DistillerURLFetcherFactory() {}
  virtual DistillerURLFetcher* CreateDistillerURLFetcher() const;

 private:
  net::URLRequestContextGetter* context_getter_;
};

class DistillerURLFetcher : public net::URLFetcherDelegate {
 public:
  explicit DistillerURLFetcher(net::URLRequestContextGetter* context_getter);
  virtual ~DistillerURLFetcher();

  
  typedef base::Callback<void(const std::string& data)> URLFetcherCallback;

  
  virtual void FetchURL(const std::string& url,
                        const URLFetcherCallback& callback);

 protected:
  virtual net::URLFetcher* CreateURLFetcher(
      net::URLRequestContextGetter* context_getter,
      const std::string& url);

 private:
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  scoped_ptr<net::URLFetcher> url_fetcher_;
  URLFetcherCallback callback_;
  net::URLRequestContextGetter* context_getter_;
  DISALLOW_COPY_AND_ASSIGN(DistillerURLFetcher);
};

}  

#endif  
