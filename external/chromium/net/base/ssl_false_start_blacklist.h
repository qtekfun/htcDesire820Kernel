// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_FALSE_START_BLACKLIST_H_
#define NET_BASE_SSL_FALSE_START_BLACKLIST_H_

#include "base/basictypes.h"

namespace net {

class SSLFalseStartBlacklist {
 public:
  
  
  static bool IsMember(const char* host);

  
  static unsigned Hash(const char* str) {
    
    
    
    
    const unsigned char* in = reinterpret_cast<const unsigned char*>(str);
    unsigned hash = 5381;
    unsigned char c;

    while ((c = *in++))
      hash = ((hash << 5) + hash) ^ c;
    return hash;
  }

  
  
  
  
  
  static const char* LastTwoLabels(const char* host) {
    
    const size_t len = strlen(host);
    if (len == 0)
      return NULL;

    unsigned dots_found = 0;
    size_t i;
    for (i = len - 1; i < len; i--) {
      if (host[i] == '.') {
        dots_found++;
        if (dots_found == 2) {
          i++;
          break;
        }
      }
    }

    if (i > len)
      i = 0;

    if (dots_found == 0)
      return NULL;  
    if (dots_found == 1) {
      if (host[0] == '.')
        return NULL;  
    }

    return &host[i];
  }

  
  
  static const unsigned kBuckets = 128;

 private:
  
  
  

  
  
  static const uint32 kHashTable[kBuckets + 1];
  
  
  
  
  
  
  
  
  static const char kHashData[];
};

}  

#endif  
