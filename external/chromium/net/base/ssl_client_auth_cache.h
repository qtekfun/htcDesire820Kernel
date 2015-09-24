// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_CLIENT_AUTH_CACHE_H_
#define NET_BASE_SSL_CLIENT_AUTH_CACHE_H_
#pragma once

#include <string>
#include <map>

#include "base/memory/ref_counted.h"
#include "net/base/cert_database.h"

namespace net {

class X509Certificate;

class SSLClientAuthCache : public CertDatabase::Observer {
 public:
  SSLClientAuthCache();
  ~SSLClientAuthCache();

  
  
  
  
  
  bool Lookup(const std::string& server,
              scoped_refptr<X509Certificate>* certificate);

  
  // a client certificate for |server|, it will be overwritten. A NULL
  
  
  void Add(const std::string& server, X509Certificate* client_cert);

  
  void Remove(const std::string& server);

  
  virtual void OnUserCertAdded(const X509Certificate* cert);

 private:
  typedef std::string AuthCacheKey;
  typedef scoped_refptr<X509Certificate> AuthCacheValue;
  typedef std::map<AuthCacheKey, AuthCacheValue> AuthCacheMap;

  
  AuthCacheMap cache_;
};

}  

#endif  
