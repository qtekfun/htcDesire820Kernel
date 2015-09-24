// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_AUTH_CACHE_H_
#define NET_FTP_FTP_AUTH_CACHE_H_
#pragma once

#include <list>

#include "base/string16.h"
#include "googleurl/src/gurl.h"

namespace net {

class FtpAuthCache {
 public:
  
  static const size_t kMaxEntries;

  struct Entry {
    Entry(const GURL& origin, const string16& username,
          const string16& password);
    ~Entry();

    const GURL origin;
    string16 username;
    string16 password;
  };

  FtpAuthCache();
  ~FtpAuthCache();

  
  Entry* Lookup(const GURL& origin);

  
  
  // overwritten.
  void Add(const GURL& origin, const string16& username,
           const string16& password);

  
  
  void Remove(const GURL& origin, const string16& username,
              const string16& password);

 private:
  typedef std::list<Entry> EntryList;

  
  
  EntryList entries_;
};

}  

#endif  
