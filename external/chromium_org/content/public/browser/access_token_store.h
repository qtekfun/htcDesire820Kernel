// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_BROWSER_ACCESS_TOKEN_STORE_H_
#define CONTENT_PUBLIC_BROWSER_ACCESS_TOKEN_STORE_H_

#include <map>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

class GURL;

namespace net {
class URLRequestContextGetter;
}

namespace content {

class AccessTokenStore : public base::RefCountedThreadSafe<AccessTokenStore> {
 public:
  
  typedef std::map<GURL, base::string16> AccessTokenSet;
  typedef base::Callback<void(AccessTokenSet, net::URLRequestContextGetter*)>
      LoadAccessTokensCallbackType;

  
  
  
  
  
  
  virtual void LoadAccessTokens(
      const LoadAccessTokensCallbackType& callback) = 0;

  virtual void SaveAccessToken(
      const GURL& server_url, const base::string16& access_token) = 0;

 protected:
  friend class base::RefCountedThreadSafe<AccessTokenStore>;
  CONTENT_EXPORT AccessTokenStore() {}
  CONTENT_EXPORT virtual ~AccessTokenStore() {}
};

}  

#endif  
