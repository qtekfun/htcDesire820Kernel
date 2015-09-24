// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_CACHE_H_
#define NET_HTTP_HTTP_AUTH_CACHE_H_

#include <list>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#include "net/http/http_auth.h"
#include "url/gurl.h"

namespace net {

class NET_EXPORT_PRIVATE HttpAuthCache {
 public:
  class NET_EXPORT_PRIVATE Entry {
   public:
    ~Entry();

    const GURL& origin() const {
      return origin_;
    }

    
    const std::string realm() const {
      return realm_;
    }

    
    HttpAuth::Scheme scheme() const {
      return scheme_;
    }

    
    const std::string auth_challenge() const {
      return auth_challenge_;
    }

    
    const AuthCredentials& credentials() const {
      return credentials_;
    }

    int IncrementNonceCount() {
      return ++nonce_count_;
    }

    void UpdateStaleChallenge(const std::string& auth_challenge);

   private:
    friend class HttpAuthCache;
    FRIEND_TEST_ALL_PREFIXES(HttpAuthCacheTest, AddPath);
    FRIEND_TEST_ALL_PREFIXES(HttpAuthCacheTest, AddToExistingEntry);

    typedef std::list<std::string> PathList;

    Entry();

    
    
    void AddPath(const std::string& path);

    
    
    
    
    
    
    
    
    bool HasEnclosingPath(const std::string& dir, size_t* path_len);

    
    GURL origin_;
    std::string realm_;
    HttpAuth::Scheme scheme_;

    
    std::string auth_challenge_;
    AuthCredentials credentials_;

    int nonce_count_;

    
    PathList paths_;
  };

  
  
  
  
  enum { kMaxNumPathsPerRealmEntry = 10 };
  enum { kMaxNumRealmEntries = 10 };

  HttpAuthCache();
  ~HttpAuthCache();

  
  
  
  
  
  
  Entry* Lookup(const GURL& origin,
                const std::string& realm,
                HttpAuth::Scheme scheme);

  
  
  
  
  
  
  
  Entry* LookupByPath(const GURL& origin, const std::string& path);

  
  
  
  
  
  
  
  
  
  
  
  Entry* Add(const GURL& origin,
             const std::string& realm,
             HttpAuth::Scheme scheme,
             const std::string& auth_challenge,
             const AuthCredentials& credentials,
             const std::string& path);

  
  
  
  
  
  
  
  bool Remove(const GURL& origin,
              const std::string& realm,
              HttpAuth::Scheme scheme,
              const AuthCredentials& credentials);

  
  
  
  
  
  bool UpdateStaleChallenge(const GURL& origin,
                            const std::string& realm,
                            HttpAuth::Scheme scheme,
                            const std::string& auth_challenge);

  
  void UpdateAllFrom(const HttpAuthCache& other);

 private:
  typedef std::list<Entry> EntryList;
  EntryList entries_;
};

}  

#endif  
