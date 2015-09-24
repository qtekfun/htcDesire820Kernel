// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_CACHE_H_
#define NET_HTTP_HTTP_AUTH_CACHE_H_
#pragma once

#include <list>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "net/http/http_auth.h"

namespace net {

class HttpAuthCache {
 public:
  class Entry;

  
  
  
  
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
             const string16& username,
             const string16& password,
             const std::string& path);

  
  
  
  
  
  
  
  
  bool Remove(const GURL& origin,
              const std::string& realm,
              HttpAuth::Scheme scheme,
              const string16& username,
              const string16& password);

  
  
  
  
  
  bool UpdateStaleChallenge(const GURL& origin,
                            const std::string& realm,
                            HttpAuth::Scheme scheme,
                            const std::string& auth_challenge);

 private:
  typedef std::list<Entry> EntryList;
  EntryList entries_;
};

class HttpAuthCache::Entry {
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

  
  const string16 username() const {
    return username_;
  }

  
  const string16 password() const {
    return password_;
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
  string16 username_;
  string16 password_;

  int nonce_count_;

  
  PathList paths_;
};

}  

#endif  
