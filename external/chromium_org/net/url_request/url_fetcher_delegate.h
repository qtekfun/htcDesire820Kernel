// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_FETCHER_DELEGATE_H_
#define NET_URL_REQUEST_URL_FETCHER_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class URLFetcher;

class NET_EXPORT URLFetcherDelegate {
 public:
  
  
  virtual void OnURLFetchComplete(const URLFetcher* source) = 0;

  
  
  
  virtual void OnURLFetchDownloadProgress(const URLFetcher* source,
                                          int64 current, int64 total);

  
  
  
  virtual void OnURLFetchUploadProgress(const URLFetcher* source,
                                        int64 current, int64 total);

 protected:
  virtual ~URLFetcherDelegate();
};

}  

#endif  
