// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UTIL_GTL_LINKED_HASH_MAP_H_
#define UTIL_GTL_LINKED_HASH_MAP_H_

#include <list>
#include <utility>

#include "base/containers/hash_tables.h"
#include "base/logging.h"

template<class Key, class Value>
class linked_hash_map {
 private:
  typedef std::list<std::pair<Key, Value> > ListType;
  typedef base::hash_map<Key, typename ListType::iterator> MapType;

 public:
  typedef typename ListType::iterator iterator;
  typedef typename ListType::reverse_iterator reverse_iterator;
  typedef typename ListType::const_iterator const_iterator;
  typedef typename ListType::const_reverse_iterator const_reverse_iterator;
  typedef typename MapType::key_type key_type;
  typedef typename ListType::value_type value_type;
  typedef typename ListType::size_type size_type;

  linked_hash_map() : map_(), list_() {
  }

  
  
  iterator begin() {
    return list_.begin();
  }
  const_iterator begin() const {
    return list_.begin();
  }

  
  iterator end() {
    return list_.end();
  }
  const_iterator end() const {
    return list_.end();
  }

  
  
  reverse_iterator rbegin() {
    return list_.rbegin();
  }
  const_reverse_iterator rbegin() const {
    return list_.rbegin();
  }

  
  reverse_iterator rend() {
    return list_.rend();
  }
  const_reverse_iterator rend() const {
    return list_.rend();
  }

  
  void clear() {
    map_.clear();
    list_.clear();
  }

  
  bool empty() const {
    return list_.empty();
  }

  
  
  size_type erase(const Key& key) {
    typename MapType::iterator found = map_.find(key);
    if (found == map_.end()) return 0;

    list_.erase(found->second);
    map_.erase(found);

    return 1;
  }

  
  
  
  void erase(iterator position) {
    typename MapType::iterator found = map_.find(position->first);
    CHECK(found->second == position)
        << "Inconsisent iterator for map and list, or the iterator is invalid.";

    list_.erase(position);
    map_.erase(found);
  }

  
  void erase(iterator first, iterator last) {
    while (first != last && first != end()) {
      erase(first++);
    }
  }

  
  
  
  iterator find(const Key& key) {
    typename MapType::iterator found = map_.find(key);
    if (found == map_.end()) {
      return end();
    }
    return found->second;
  }

  const_iterator find(const Key& key) const {
    typename MapType::const_iterator found = map_.find(key);
    if (found == map_.end()) {
      return end();
    }
    return found->second;
  }

  
  
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    std::pair<typename MapType::iterator, typename MapType::iterator> eq_range =
        map_.equal_range(key);

    return std::make_pair(eq_range.first->second, eq_range.second->second);
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    std::pair<typename MapType::const_iterator,
        typename MapType::const_iterator> eq_range =
        map_.equal_range(key);
    const const_iterator& start_iter = eq_range.first != map_.end() ?
        eq_range.first->second : end();
    const const_iterator& end_iter = eq_range.second != map_.end() ?
        eq_range.second->second : end();

    return std::make_pair(start_iter, end_iter);
  }

  
  
  Value& operator[](const key_type& key) {
    return (*((this->insert(std::make_pair(key, Value()))).first)).second;
  }

  
  std::pair<iterator, bool> insert(const std::pair<Key, Value>& pair) {
    
    
    
    typename MapType::iterator found = map_.find(pair.first);
    if (found != map_.end()) return std::make_pair(found->second, false);

    
    list_.push_back(pair);

    
    
    typename ListType::iterator last = list_.end();
    --last;

    CHECK(map_.insert(std::make_pair(pair.first, last)).second)
        << "Map and list are inconsistent";

    return std::make_pair(last, true);
  }

  size_type size() const {
    return list_.size();
  }

  void swap(linked_hash_map& other) {
    map_.swap(other.map_);
    list_.swap(other.list_);
  }

 private:
  
  MapType map_;

  
  ListType list_;
};

#endif  
