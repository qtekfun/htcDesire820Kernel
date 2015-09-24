// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_CONTEXT_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_CONTEXT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "build/build_config.h"
#include "content/common/content_export.h"

class GURL;

namespace appcache {
class AppCacheService;
}

namespace net {
class ClientCertStore;
class HostResolver;
class KeygenHandler;
class URLRequestContext;
}

namespace content {

class CONTENT_EXPORT ResourceContext : public base::SupportsUserData {
 public:
#if defined(OS_IOS)
  virtual ~ResourceContext() {}
#else
  ResourceContext();
  virtual ~ResourceContext();
#endif
  virtual net::HostResolver* GetHostResolver() = 0;

  
  
  
  virtual net::URLRequestContext* GetRequestContext() = 0;

  
  virtual scoped_ptr<net::ClientCertStore> CreateClientCertStore();

  
  
  virtual void CreateKeygenHandler(
      uint32 key_size_in_bits,
      const std::string& challenge_string,
      const GURL& url,
      const base::Callback<void(scoped_ptr<net::KeygenHandler>)>& callback);

  
  
  
  virtual bool AllowMicAccess(const GURL& origin) = 0;

  
  
  
  virtual bool AllowCameraAccess(const GURL& origin) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<std::string()> SaltCallback;
  virtual SaltCallback GetMediaDeviceIDSalt();
};

}  

#endif  
