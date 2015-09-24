// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_URL_SECURITY_MANAGER_H_
#define NET_HTTP_URL_SECURITY_MANAGER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class HttpAuthFilter;

class NET_EXPORT URLSecurityManager {
 public:
  URLSecurityManager() {}
  virtual ~URLSecurityManager() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static URLSecurityManager* Create(const HttpAuthFilter* whitelist_default,
                                    const HttpAuthFilter* whitelist_delegate);

  
  
  virtual bool CanUseDefaultCredentials(const GURL& auth_origin) const = 0;

  
  
  virtual bool CanDelegate(const GURL& auth_origin) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(URLSecurityManager);
};

class URLSecurityManagerWhitelist : public URLSecurityManager {
 public:
  
  URLSecurityManagerWhitelist(const HttpAuthFilter* whitelist_default,
                              const HttpAuthFilter* whitelist_delegation);
  virtual ~URLSecurityManagerWhitelist();

  
  virtual bool CanUseDefaultCredentials(const GURL& auth_origin) const OVERRIDE;
  virtual bool CanDelegate(const GURL& auth_origin) const OVERRIDE;

 private:
  scoped_ptr<const HttpAuthFilter> whitelist_default_;
  scoped_ptr<const HttpAuthFilter> whitelist_delegate_;

  DISALLOW_COPY_AND_ASSIGN(URLSecurityManagerWhitelist);
};

}  

#endif  
