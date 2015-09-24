// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_FETCHER_FACTORY_H_
#define NET_URL_REQUEST_URL_FETCHER_FACTORY_H_

#include "net/url_request/url_fetcher.h"

namespace net {
class URLFetcherDelegate;

class URLFetcherFactory {
 public:
  virtual URLFetcher* CreateURLFetcher(
      int id,
      const GURL& url,
      URLFetcher::RequestType request_type,
      URLFetcherDelegate* delegate) = 0;

 protected:
  virtual ~URLFetcherFactory() {}
};

}  

#endif  
