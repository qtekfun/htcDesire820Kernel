// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_EXPIRING_CACHE_H_
#define NET_BASE_EXPIRING_CACHE_H_

#include <map>
#include <utility>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"

namespace net {

template <typename KeyType,
          typename ValueType,
          typename ExpirationType>
class NoopEvictionHandler {
 public:
  void Handle(const KeyType& key,
              const ValueType& value,
              const ExpirationType& expiration,
              const ExpirationType& now,
              bool onGet) const {
  }
};

template <typename KeyType,
          typename ValueType,
          typename ExpirationType,
          typename ExpirationCompare,
          typename EvictionHandler = NoopEvictionHandler<KeyType,
                                                         ValueType,
                                                         ExpirationType> >
class ExpiringCache {
 private:
  
  
  
  

  
  typedef std::pair<ValueType, ExpirationType> Entry;
  typedef std::map<KeyType, Entry> EntryMap;

 public:
  typedef KeyType key_type;
  typedef ValueType value_type;
  typedef ExpirationType expiration_type;

  
  class Iterator {
   public:
    explicit Iterator(const ExpiringCache& cache)
        : cache_(cache),
          it_(cache_.entries_.begin()) {
    }
    ~Iterator() {}

    bool HasNext() const { return it_ != cache_.entries_.end(); }
    void Advance() { ++it_; }

    const KeyType& key() const { return it_->first; }
    const ValueType& value() const { return it_->second.first; }
    const ExpirationType& expiration() const { return it_->second.second; }

   private:
    const ExpiringCache& cache_;

    
    
    typedef typename ExpiringCache::EntryMap EntryMap;
    typename EntryMap::const_iterator it_;
  };


  
  explicit ExpiringCache(size_t max_entries) : max_entries_(max_entries) {}
  ~ExpiringCache() {}

  
  
  
  
  
  const ValueType* Get(const KeyType& key, const ExpirationType& now) {
    typename EntryMap::iterator it = entries_.find(key);
    if (it == entries_.end())
      return NULL;

    
    if (!expiration_comp_(now, it->second.second)) {
      Evict(it, now, true);
      return NULL;
    }

    return &it->second.first;
  }

  
  void Put(const KeyType& key,
           const ValueType& value,
           const ExpirationType& now,
           const ExpirationType& expiration) {
    typename EntryMap::iterator it = entries_.find(key);
    if (it == entries_.end()) {
      
      if (entries_.size() == max_entries_ )
        Compact(now);

      
      entries_.insert(std::make_pair(key, Entry(value, expiration)));
    } else {
      
      it->second.first = value;
      it->second.second = expiration;
    }
  }

  
  void Clear() {
    entries_.clear();
  }

  
  size_t size() const { return entries_.size(); }

  
  size_t max_entries() const { return max_entries_; }

  bool empty() const { return entries_.empty(); }

 private:
  FRIEND_TEST_ALL_PREFIXES(ExpiringCacheTest, Compact);
  FRIEND_TEST_ALL_PREFIXES(ExpiringCacheTest, CustomFunctor);

  
  void Compact(const ExpirationType& now) {
    
    typename EntryMap::iterator it;
    for (it = entries_.begin(); it != entries_.end(); ) {
      if (!expiration_comp_(now, it->second.second)) {
        Evict(it++, now, false);
      } else {
        ++it;
      }
    }

    if (entries_.size() < max_entries_)
      return;

    
    for (it = entries_.begin();
         it != entries_.end() && entries_.size() >= max_entries_;) {
      Evict(it++, now, false);
    }
  }

  void Evict(typename EntryMap::iterator it,
             const ExpirationType& now,
             bool on_get) {
    eviction_handler_.Handle(it->first, it->second.first, it->second.second,
                             now, on_get);
    entries_.erase(it);
  }

  
  size_t max_entries_;

  EntryMap entries_;
  ExpirationCompare expiration_comp_;
  EvictionHandler eviction_handler_;

  DISALLOW_COPY_AND_ASSIGN(ExpiringCache);
};

}  

#endif  
