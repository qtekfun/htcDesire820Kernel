// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_INTERCEPTOR_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_INTERCEPTOR_H_

#include <string>

#include "base/basictypes.h"

class GURL;

namespace base {
class FilePath;
}

namespace content {

class URLRequestPrepackagedInterceptor {
 public:
  
  
  URLRequestPrepackagedInterceptor(const std::string& scheme,
                                   const std::string& hostname);
  virtual ~URLRequestPrepackagedInterceptor();

  
  
  
  void SetResponse(const GURL& url, const base::FilePath& path);

  
  
  void SetResponseIgnoreQuery(const GURL& url, const base::FilePath& path);

  
  int GetHitCount();

 private:
  class Delegate;

  const std::string scheme_;
  const std::string hostname_;

  
  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestPrepackagedInterceptor);
};

class URLLocalHostRequestPrepackagedInterceptor
    : public URLRequestPrepackagedInterceptor {
 public:
  URLLocalHostRequestPrepackagedInterceptor();

 private:
  DISALLOW_COPY_AND_ASSIGN(URLLocalHostRequestPrepackagedInterceptor);
};

}  

#endif  
