// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_POLICY_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_POLICY_H_

class GURL;

namespace appcache {

class AppCachePolicy {
 public:
  AppCachePolicy() {}

  
  
  
  virtual bool CanLoadAppCache(const GURL& manifest_url,
                               const GURL& first_party) = 0;

  
  virtual bool CanCreateAppCache(const GURL& manifest_url,
                                 const GURL& first_party) = 0;

 protected:
  ~AppCachePolicy() {}
};

}  

#endif  
