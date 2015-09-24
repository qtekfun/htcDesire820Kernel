// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_SESSION_CACHE_OPENSSL_H
#define NET_SOCKET_SSL_SESSION_CACHE_OPENSSL_H

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"

typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_st SSL;

namespace net {

class SSLSessionCacheOpenSSLImpl;

class NET_EXPORT SSLSessionCacheOpenSSL {
 public:
  
  
  typedef std::string GetSessionKeyFunction(const SSL* ssl);

  
  
  
  
  
  
  
  struct Config {
    GetSessionKeyFunction* key_func;
    size_t max_entries;
    size_t expiration_check_count;
    int timeout_seconds;
  };

  SSLSessionCacheOpenSSL() : impl_(NULL) {}

  
  
  
  
  
  
  
  
  SSLSessionCacheOpenSSL(SSL_CTX* ctx, const Config& config) : impl_(NULL) {
    Reset(ctx, config);
  }

  
  
  ~SSLSessionCacheOpenSSL();

  
  void Reset(SSL_CTX* ctx, const Config& config);

  size_t size() const;

  
  
  
  
  
  
  
  
  
  
  bool SetSSLSession(SSL* ssl);

  
  
  
  
  
  
  
  
  
  
  bool SetSSLSessionWithKey(SSL* ssl, const std::string& cache_key);

  
  
  
  
  
  
  void MarkSSLSessionAsGood(SSL* ssl);

  
  
  void Flush();

  
  static const int kDefaultTimeoutSeconds = 60 * 60;
  static const size_t kMaxEntries = 1024;
  static const size_t kMaxExpirationChecks = 256;

 private:
  DISALLOW_COPY_AND_ASSIGN(SSLSessionCacheOpenSSL);

  SSLSessionCacheOpenSSLImpl* impl_;
};

}  

#endif  
