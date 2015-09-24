// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_JTL_FOUNDATION_H_
#define CHROME_BROWSER_PROFILE_RESETTER_JTL_FOUNDATION_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "crypto/hmac.h"

namespace jtl_foundation {


enum OpCodes {
  
  
  
  
  
  NAVIGATE = 0x00,
  
  
  
  
  NAVIGATE_ANY = 0x01,
  
  
  
  NAVIGATE_BACK = 0x02,
  
  
  
  
  STORE_BOOL = 0x10,
  
  
  
  
  
  
  
  COMPARE_STORED_BOOL = 0x11,
  
  
  STORE_HASH = 0x12,
  
  
  COMPARE_STORED_HASH = 0x13,
  
  
  
  
  
  STORE_NODE_BOOL = 0x14,
  
  
  
  
  
  STORE_NODE_HASH = 0x15,
  
  
  
  
  
  
  
  
  
  
  STORE_NODE_EFFECTIVE_SLD_HASH = 0x16,
  
  
  
  
  
  
  COMPARE_NODE_BOOL = 0x20,
  
  
  
  
  
  
  
  COMPARE_NODE_HASH = 0x21,
  
  COMPARE_NODE_HASH_NOT = 0x22,
  
  
  
  
  
  
  
  COMPARE_NODE_TO_STORED_BOOL = 0x23,
  
  
  
  
  
  
  
  COMPARE_NODE_TO_STORED_HASH = 0x24,
  
  
  
  
  
  
  
  
  
  COMPARE_NODE_SUBSTRING = 0x25,
  
  STOP_EXECUTING_SENTENCE = 0x30,
  
  END_OF_SENTENCE = 0x31
};

const size_t kHashSizeInBytes = 32u;

class Hasher {
 public:
  explicit Hasher(const std::string& seed);
  ~Hasher();

  std::string GetHash(const std::string& input) const;

  static bool IsHash(const std::string& maybe_hash);

 private:
  crypto::HMAC hmac_;
  mutable std::map<std::string, std::string> cached_hashes_;
  DISALLOW_COPY_AND_ASSIGN(Hasher);
};

}  

#endif  
