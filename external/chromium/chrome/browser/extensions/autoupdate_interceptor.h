// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_AUTOUPDATE_INTERCEPTOR_H_
#define CHROME_BROWSER_EXTENSIONS_AUTOUPDATE_INTERCEPTOR_H_
#pragma once

#include <map>
#include <string>

#include "googleurl/src/gurl.h"
#include "net/url_request/url_request.h"

class AutoUpdateInterceptor
    : public net::URLRequest::Interceptor,
      public base::RefCountedThreadSafe<AutoUpdateInterceptor> {
 public:
  AutoUpdateInterceptor();

  
  
  virtual net::URLRequestJob* MaybeIntercept(net::URLRequest* request);

  
  
  
  
  void SetResponse(const std::string url, const FilePath& path);

  
  void SetResponseOnIOThread(const std::string url, const FilePath& path);

 private:
  friend class base::RefCountedThreadSafe<AutoUpdateInterceptor>;

  virtual ~AutoUpdateInterceptor();

  std::map<GURL, FilePath> responses_;

  DISALLOW_COPY_AND_ASSIGN(AutoUpdateInterceptor);
};

#endif  
