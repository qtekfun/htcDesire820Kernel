// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// NOTE: While all operations are O(1), this code is written for

#ifndef BASE_CONTAINERS_MRU_CACHE_H_
#define BASE_CONTAINERS_MRU_CACHE_H_

#include <list>
#include <map>
#include <utility>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"

namespace base {


template <class KeyType, class ValueType>
struct MRUCacheStandardMap {
  typedef std::map<KeyType, ValueType> Type;
};

template <class KeyType, class PayloadType, class DeletorType,
          template <typename, typename> class MapType = MRUCacheStandardMap>
class MRUCacheBase {
 public:
  
  
  typedef std::pair<KeyType, PayloadType> value_type;

 private:
  typedef std::list<value_type> PayloadList;
  typedef typename MapType<KeyType,
                           typename PayloadList::iterator>::Type KeyIndex;

 public:
  typedef typename PayloadList::size_type size_type;

  typedef typename PayloadList::iterator iterator;
  typedef typename PayloadList::const_iterator const_iterator;
  typedef typename PayloadList::reverse_iterator reverse_iterator;
  typedef typename PayloadList::const_reverse_iterator const_reverse_iterator;

  enum { NO_AUTO_EVICT = 0 };

  
  
  
  
  explicit MRUCacheBase(size_type max_size) : max_size_(max_size) {
  }

  MRUCacheBase(size_type max_size, const DeletorType& deletor)
      : max_size_(max_size), deletor_(deletor) {
  }

  virtual ~MRUCacheBase() {
    iterator i = begin();
    while (i != end())
      i = Erase(i);
  }

  size_type max_size() const { return max_size_; }

  
  
  
  
  
  
  iterator Put(const KeyType& key, const PayloadType& payload) {
    
    typename KeyIndex::iterator index_iter = index_.find(key);
    if (index_iter != index_.end()) {
      
      
      Erase(index_iter->second);
    } else if (max_size_ != NO_AUTO_EVICT) {
      
      
      ShrinkToSize(max_size_ - 1);
    }

    ordering_.push_front(value_type(key, payload));
    index_.insert(std::make_pair(key, ordering_.begin()));
    return ordering_.begin();
  }

  
  
  
  
  
  iterator Get(const KeyType& key) {
    typename KeyIndex::iterator index_iter = index_.find(key);
    if (index_iter == index_.end())
      return end();
    typename PayloadList::iterator iter = index_iter->second;

    
    ordering_.splice(ordering_.begin(), ordering_, iter);
    return ordering_.begin();
  }

  
  
  iterator Peek(const KeyType& key) {
    typename KeyIndex::const_iterator index_iter = index_.find(key);
    if (index_iter == index_.end())
      return end();
    return index_iter->second;
  }

  const_iterator Peek(const KeyType& key) const {
    typename KeyIndex::const_iterator index_iter = index_.find(key);
    if (index_iter == index_.end())
      return end();
    return index_iter->second;
  }

  
  
  iterator Erase(iterator pos) {
    deletor_(pos->second);
    index_.erase(pos->first);
    return ordering_.erase(pos);
  }

  
  
  reverse_iterator Erase(reverse_iterator pos) {
    
    
    
    return reverse_iterator(Erase((++pos).base()));
  }

  
  
  void ShrinkToSize(size_type new_size) {
    for (size_type i = size(); i > new_size; i--)
      Erase(rbegin());
  }

  
  void Clear() {
    for (typename PayloadList::iterator i(ordering_.begin());
         i != ordering_.end(); ++i)
      deletor_(i->second);
    index_.clear();
    ordering_.clear();
  }

  
  size_type size() const {
    
    
    DCHECK(index_.size() == ordering_.size());
    return index_.size();
  }

  
  
  
  
  
  
  iterator begin() { return ordering_.begin(); }
  const_iterator begin() const { return ordering_.begin(); }
  iterator end() { return ordering_.end(); }
  const_iterator end() const { return ordering_.end(); }

  reverse_iterator rbegin() { return ordering_.rbegin(); }
  const_reverse_iterator rbegin() const { return ordering_.rbegin(); }
  reverse_iterator rend() { return ordering_.rend(); }
  const_reverse_iterator rend() const { return ordering_.rend(); }

  bool empty() const { return ordering_.empty(); }

 private:
  PayloadList ordering_;
  KeyIndex index_;

  size_type max_size_;

  DeletorType deletor_;

  DISALLOW_COPY_AND_ASSIGN(MRUCacheBase);
};


template<class PayloadType>
class MRUCacheNullDeletor {
 public:
  void operator()(PayloadType& payload) {
  }
};

template <class KeyType, class PayloadType>
class MRUCache : public MRUCacheBase<KeyType,
                                     PayloadType,
                                     MRUCacheNullDeletor<PayloadType> > {
 private:
  typedef MRUCacheBase<KeyType, PayloadType,
      MRUCacheNullDeletor<PayloadType> > ParentType;

 public:
  
  explicit MRUCache(typename ParentType::size_type max_size)
      : ParentType(max_size) {
  }
  virtual ~MRUCache() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MRUCache);
};


template<class PayloadType>
class MRUCachePointerDeletor {
 public:
  void operator()(PayloadType& payload) {
    delete payload;
  }
};

template <class KeyType, class PayloadType>
class OwningMRUCache
    : public MRUCacheBase<KeyType,
                          PayloadType,
                          MRUCachePointerDeletor<PayloadType> > {
 private:
  typedef MRUCacheBase<KeyType, PayloadType,
      MRUCachePointerDeletor<PayloadType> > ParentType;

 public:
  
  explicit OwningMRUCache(typename ParentType::size_type max_size)
      : ParentType(max_size) {
  }
  virtual ~OwningMRUCache() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(OwningMRUCache);
};


template <class KeyType, class ValueType>
struct MRUCacheHashMap {
  typedef base::hash_map<KeyType, ValueType> Type;
};

template <class KeyType, class PayloadType>
class HashingMRUCache : public MRUCacheBase<KeyType,
                                            PayloadType,
                                            MRUCacheNullDeletor<PayloadType>,
                                            MRUCacheHashMap> {
 private:
  typedef MRUCacheBase<KeyType, PayloadType,
                       MRUCacheNullDeletor<PayloadType>,
                       MRUCacheHashMap> ParentType;

 public:
  
  explicit HashingMRUCache(typename ParentType::size_type max_size)
      : ParentType(max_size) {
  }
  virtual ~HashingMRUCache() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(HashingMRUCache);
};

}  

#endif  
