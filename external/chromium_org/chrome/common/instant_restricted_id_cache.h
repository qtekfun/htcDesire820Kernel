// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_INSTANT_RESTRICTED_ID_CACHE_H_
#define CHROME_COMMON_INSTANT_RESTRICTED_ID_CACHE_H_

#include <set>
#include <utility>
#include <vector>

#include "base/containers/mru_cache.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "chrome/common/instant_types.h"


template <typename T>
class InstantRestrictedIDCache {
 public:
  typedef std::pair<InstantRestrictedID, T> ItemIDPair;
  typedef std::vector<T> ItemVector;
  typedef std::vector<ItemIDPair> ItemIDVector;

  explicit InstantRestrictedIDCache(size_t max_cache_size);
  ~InstantRestrictedIDCache();

  
  
  
  void AddItems(const ItemVector& items);

  
  
  
  void AddItemsWithRestrictedID(const ItemIDVector& items);

  
  
  void GetCurrentItems(ItemIDVector* items) const;

  
  
  bool GetItemWithRestrictedID(InstantRestrictedID restricted_id,
                               T* item) const;

 private:
  FRIEND_TEST_ALL_PREFIXES(InstantRestrictedIDCacheTest, AutoIDGeneration);
  FRIEND_TEST_ALL_PREFIXES(InstantRestrictedIDCacheTest, CrazyIDGeneration);
  FRIEND_TEST_ALL_PREFIXES(InstantRestrictedIDCacheTest, ManualIDGeneration);
  FRIEND_TEST_ALL_PREFIXES(InstantRestrictedIDCacheTest, MixIDGeneration);
  FRIEND_TEST_ALL_PREFIXES(InstantRestrictedIDCacheTest, AddEmptySet);
  FRIEND_TEST_ALL_PREFIXES(InstantRestrictedIDCacheTest,
                           AddItemsWithRestrictedID);

  typedef base::MRUCache<InstantRestrictedID, T> CacheImpl;

  mutable CacheImpl cache_;
  typename CacheImpl::reverse_iterator last_add_start_;
  InstantRestrictedID last_restricted_id_;

  DISALLOW_COPY_AND_ASSIGN(InstantRestrictedIDCache);
};

template <typename T>
InstantRestrictedIDCache<T>::InstantRestrictedIDCache(size_t max_cache_size)
    : cache_(max_cache_size),
      last_add_start_(cache_.rend()),
      last_restricted_id_(0) {
  DCHECK(max_cache_size);
}

template <typename T>
InstantRestrictedIDCache<T>::~InstantRestrictedIDCache() {
}

template <typename T>
void InstantRestrictedIDCache<T>::AddItems(const ItemVector& items) {
  DCHECK_LE(items.size(), cache_.max_size());

  if (items.empty()) {
    last_add_start_ = cache_.rend();
    return;
  }

  for (size_t i = 0; i < items.size(); ++i) {
    InstantRestrictedID id = ++last_restricted_id_;
    cache_.Put(id, items[i]);
    if (i == 0)
      last_add_start_ = --cache_.rend();
  }
}

template <typename T>
void InstantRestrictedIDCache<T>::AddItemsWithRestrictedID(
    const ItemIDVector& items) {
  DCHECK_LE(items.size(), cache_.max_size());

  if (items.empty()) {
    last_add_start_ = cache_.rend();
    return;
  }

  std::set<InstantRestrictedID> ids_added;
  for (size_t i = 0; i < items.size(); ++i) {
    const ItemIDPair& item_id = items[i];

    DCHECK(ids_added.find(item_id.first) == ids_added.end());
    ids_added.insert(item_id.first);

    cache_.Put(item_id.first, item_id.second);
    last_restricted_id_ = std::max(item_id.first, last_restricted_id_);
  }

  
  
  
  last_add_start_ = cache_.rend();
  for (size_t i = 0; i < items.size(); ++i)
    --last_add_start_;
}

template <typename T>
void InstantRestrictedIDCache<T>::GetCurrentItems(ItemIDVector* items) const {
  items->clear();

  for (typename CacheImpl::reverse_iterator it = last_add_start_;
       it != cache_.rend(); ++it) {
    items->push_back(std::make_pair(it->first, it->second));
  }
}

template <typename T>
bool InstantRestrictedIDCache<T>::GetItemWithRestrictedID(
    InstantRestrictedID restricted_id,
    T* item) const {
  DCHECK(item);

  typename CacheImpl::const_iterator cache_it = cache_.Peek(restricted_id);
  if (cache_it == cache_.end())
    return false;
  *item = cache_it->second;
  return true;
}

#endif  
