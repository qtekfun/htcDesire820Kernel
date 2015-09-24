// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_VALIDATION_CACHE_H_
#define COMPONENTS_NACL_BROWSER_NACL_VALIDATION_CACHE_H_

#include <vector>

#include "base/containers/mru_cache.h"

class Pickle;

namespace nacl {

class NaClValidationCache {
 public:
  NaClValidationCache();
  ~NaClValidationCache();

  
  
  const std::string& GetValidationCacheKey() const {
    return validation_cache_key_;
  }

  
  bool QueryKnownToValidate(const std::string& signature, bool reorder);

  
  void SetKnownToValidate(const std::string& signature);

  void Reset();
  void Serialize(Pickle* pickle) const;
  bool Deserialize(const Pickle* pickle);

  
  size_t size() const {
    return validation_cache_.size();
  }
  void SetValidationCacheKey(std::string& key) {
    validation_cache_key_ = key;
  }
  std::vector<std::string> GetContents() const {
    std::vector<std::string> contents;
    ValidationCacheType::const_iterator iter = validation_cache_.begin();
    for (iter = validation_cache_.begin();
         iter != validation_cache_.end();
         iter++) {
      contents.push_back(iter->first);
    }
    return contents;
  }

 private:
  bool DeserializeImpl(const Pickle* pickle);

  typedef base::HashingMRUCache<std::string, bool> ValidationCacheType;
  ValidationCacheType validation_cache_;

  std::string validation_cache_key_;

  DISALLOW_COPY_AND_ASSIGN(NaClValidationCache);
};

} 

#endif  
