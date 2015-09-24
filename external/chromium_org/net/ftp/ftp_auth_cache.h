// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_AUTH_CACHE_H_
#define NET_FTP_FTP_AUTH_CACHE_H_

#include <list>

#include "net/base/auth.h"
#include "net/base/net_export.h"
#include "url/gurl.h"

namespace net {

class NET_EXPORT_PRIVATE FtpAuthCache {
 public:
  
  static const size_t kMaxEntries;

  struct Entry {
    Entry(const GURL& origin, const AuthCredentials& credentials);
    ~Entry();

    GURL origin;
    AuthCredentials credentials;
  };

  FtpAuthCache();
  ~FtpAuthCache();

  
  Entry* Lookup(const GURL& origin);

  
  // already an entry for |origin|, it will be overwritten.
  void Add(const GURL& origin, const AuthCredentials& credentials);

  
  
  void Remove(const GURL& origin, const AuthCredentials& credentials);

 private:
  typedef std::list<Entry> EntryList;

  
  
  EntryList entries_;
};

}  

#endif  
