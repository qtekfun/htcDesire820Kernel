// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_CONTAINERS_SMALL_MAP_H_
#define BASE_CONTAINERS_SMALL_MAP_H_

#include <map>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/manual_constructor.h"

namespace base {


namespace internal {

template <typename NormalMap>
class SmallMapDefaultInit {
 public:
  void operator()(ManualConstructor<NormalMap>* map) const {
    map->Init();
  }
};

template <typename M>
struct has_key_equal {
  typedef char sml;  
  typedef struct { char dummy[2]; } big;
  
  
  
  template <typename U> static big test(typename U::key_equal*);
  template <typename> static sml test(...);
  
  
  static const bool value = (sizeof(test<M>(0)) == sizeof(big));
};
template <typename M> const bool has_key_equal<M>::value;

template <typename M, bool has_key_equal_value>
struct select_equal_key {
  struct equal_key {
    bool operator()(const typename M::key_type& left,
                    const typename M::key_type& right) {
      
      typename M::key_compare comp;
      return !comp(left, right) && !comp(right, left);
    }
  };
};

template <typename KeyType, typename ValueType>
struct select_equal_key< std::map<KeyType, ValueType>, false> {
  struct equal_key {
    bool operator()(const KeyType& left, const KeyType& right) {
      return left == right;
    }
  };
};
template <typename KeyType, typename ValueType>
struct select_equal_key< base::hash_map<KeyType, ValueType>, false> {
  struct equal_key {
    bool operator()(const KeyType& left, const KeyType& right) {
      return left == right;
    }
  };
};

template <typename M>
struct select_equal_key<M, true> {
  typedef typename M::key_equal equal_key;
};

}  

template <typename NormalMap,
          int kArraySize = 4,
          typename EqualKey =
              typename internal::select_equal_key<
                  NormalMap,
                  internal::has_key_equal<NormalMap>::value>::equal_key,
          typename MapInit = internal::SmallMapDefaultInit<NormalMap> >
class SmallMap {
  
  
  
  
  COMPILE_ASSERT(kArraySize > 0, default_initial_size_should_be_positive);

 public:
  typedef typename NormalMap::key_type key_type;
  typedef typename NormalMap::mapped_type data_type;
  typedef typename NormalMap::mapped_type mapped_type;
  typedef typename NormalMap::value_type value_type;
  typedef EqualKey key_equal;

  SmallMap() : size_(0), functor_(MapInit()) {}

  explicit SmallMap(const MapInit& functor) : size_(0), functor_(functor) {}

  
  SmallMap(const SmallMap& src) {
    
    InitFrom(src);
  }
  void operator=(const SmallMap& src) {
    if (&src == this) return;

    
    
    
    
    Destroy();
    InitFrom(src);
  }
  ~SmallMap() {
    Destroy();
  }

  class const_iterator;

  class iterator {
   public:
    typedef typename NormalMap::iterator::iterator_category iterator_category;
    typedef typename NormalMap::iterator::value_type value_type;
    typedef typename NormalMap::iterator::difference_type difference_type;
    typedef typename NormalMap::iterator::pointer pointer;
    typedef typename NormalMap::iterator::reference reference;

    inline iterator(): array_iter_(NULL) {}

    inline iterator& operator++() {
      if (array_iter_ != NULL) {
        ++array_iter_;
      } else {
        ++hash_iter_;
      }
      return *this;
    }
    inline iterator operator++(int ) {
      iterator result(*this);
      ++(*this);
      return result;
    }
    inline iterator& operator--() {
      if (array_iter_ != NULL) {
        --array_iter_;
      } else {
        --hash_iter_;
      }
      return *this;
    }
    inline iterator operator--(int ) {
      iterator result(*this);
      --(*this);
      return result;
    }
    inline value_type* operator->() const {
      if (array_iter_ != NULL) {
        return array_iter_->get();
      } else {
        return hash_iter_.operator->();
      }
    }

    inline value_type& operator*() const {
      if (array_iter_ != NULL) {
        return *array_iter_->get();
      } else {
        return *hash_iter_;
      }
    }

    inline bool operator==(const iterator& other) const {
      if (array_iter_ != NULL) {
        return array_iter_ == other.array_iter_;
      } else {
        return other.array_iter_ == NULL && hash_iter_ == other.hash_iter_;
      }
    }

    inline bool operator!=(const iterator& other) const {
      return !(*this == other);
    }

    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const;

   private:
    friend class SmallMap;
    friend class const_iterator;
    inline explicit iterator(ManualConstructor<value_type>* init)
      : array_iter_(init) {}
    inline explicit iterator(const typename NormalMap::iterator& init)
      : array_iter_(NULL), hash_iter_(init) {}

    ManualConstructor<value_type>* array_iter_;
    typename NormalMap::iterator hash_iter_;
  };

  class const_iterator {
   public:
    typedef typename NormalMap::const_iterator::iterator_category
        iterator_category;
    typedef typename NormalMap::const_iterator::value_type value_type;
    typedef typename NormalMap::const_iterator::difference_type difference_type;
    typedef typename NormalMap::const_iterator::pointer pointer;
    typedef typename NormalMap::const_iterator::reference reference;

    inline const_iterator(): array_iter_(NULL) {}
    
    inline const_iterator(const iterator& other)
      : array_iter_(other.array_iter_), hash_iter_(other.hash_iter_) {}

    inline const_iterator& operator++() {
      if (array_iter_ != NULL) {
        ++array_iter_;
      } else {
        ++hash_iter_;
      }
      return *this;
    }
    inline const_iterator operator++(int ) {
      const_iterator result(*this);
      ++(*this);
      return result;
    }

    inline const_iterator& operator--() {
      if (array_iter_ != NULL) {
        --array_iter_;
      } else {
        --hash_iter_;
      }
      return *this;
    }
    inline const_iterator operator--(int ) {
      const_iterator result(*this);
      --(*this);
      return result;
    }

    inline const value_type* operator->() const {
      if (array_iter_ != NULL) {
        return array_iter_->get();
      } else {
        return hash_iter_.operator->();
      }
    }

    inline const value_type& operator*() const {
      if (array_iter_ != NULL) {
        return *array_iter_->get();
      } else {
        return *hash_iter_;
      }
    }

    inline bool operator==(const const_iterator& other) const {
      if (array_iter_ != NULL) {
        return array_iter_ == other.array_iter_;
      } else {
        return other.array_iter_ == NULL && hash_iter_ == other.hash_iter_;
      }
    }

    inline bool operator!=(const const_iterator& other) const {
      return !(*this == other);
    }

   private:
    friend class SmallMap;
    inline explicit const_iterator(
        const ManualConstructor<value_type>* init)
      : array_iter_(init) {}
    inline explicit const_iterator(
        const typename NormalMap::const_iterator& init)
      : array_iter_(NULL), hash_iter_(init) {}

    const ManualConstructor<value_type>* array_iter_;
    typename NormalMap::const_iterator hash_iter_;
  };

  iterator find(const key_type& key) {
    key_equal compare;
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        if (compare(array_[i]->first, key)) {
          return iterator(array_ + i);
        }
      }
      return iterator(array_ + size_);
    } else {
      return iterator(map()->find(key));
    }
  }

  const_iterator find(const key_type& key) const {
    key_equal compare;
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        if (compare(array_[i]->first, key)) {
          return const_iterator(array_ + i);
        }
      }
      return const_iterator(array_ + size_);
    } else {
      return const_iterator(map()->find(key));
    }
  }

  
  data_type& operator[](const key_type& key) {
    key_equal compare;

    if (size_ >= 0) {
      
      
      for (int i = size_-1; i >= 0; --i) {
        if (compare(array_[i]->first, key)) {
          return array_[i]->second;
        }
      }
      if (size_ == kArraySize) {
        ConvertToRealMap();
        return (*map_)[key];
      } else {
        array_[size_].Init(key, data_type());
        return array_[size_++]->second;
      }
    } else {
      return (*map_)[key];
    }
  }

  
  std::pair<iterator, bool> insert(const value_type& x) {
    key_equal compare;

    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        if (compare(array_[i]->first, x.first)) {
          return std::make_pair(iterator(array_ + i), false);
        }
      }
      if (size_ == kArraySize) {
        ConvertToRealMap();  
        std::pair<typename NormalMap::iterator, bool> ret = map_->insert(x);
        return std::make_pair(iterator(ret.first), ret.second);
      } else {
        array_[size_].Init(x);
        return std::make_pair(iterator(array_ + size_++), true);
      }
    } else {
      std::pair<typename NormalMap::iterator, bool> ret = map_->insert(x);
      return std::make_pair(iterator(ret.first), ret.second);
    }
  }

  
  template <class InputIterator>
  void insert(InputIterator f, InputIterator l) {
    while (f != l) {
      insert(*f);
      ++f;
    }
  }

  iterator begin() {
    if (size_ >= 0) {
      return iterator(array_);
    } else {
      return iterator(map_->begin());
    }
  }
  const_iterator begin() const {
    if (size_ >= 0) {
      return const_iterator(array_);
    } else {
      return const_iterator(map_->begin());
    }
  }

  iterator end() {
    if (size_ >= 0) {
      return iterator(array_ + size_);
    } else {
      return iterator(map_->end());
    }
  }
  const_iterator end() const {
    if (size_ >= 0) {
      return const_iterator(array_ + size_);
    } else {
      return const_iterator(map_->end());
    }
  }

  void clear() {
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        array_[i].Destroy();
      }
    } else {
      map_.Destroy();
    }
    size_ = 0;
  }

  
  void erase(const iterator& position) {
    if (size_ >= 0) {
      int i = position.array_iter_ - array_;
      array_[i].Destroy();
      --size_;
      if (i != size_) {
        array_[i].Init(*array_[size_]);
        array_[size_].Destroy();
      }
    } else {
      map_->erase(position.hash_iter_);
    }
  }

  size_t erase(const key_type& key) {
    iterator iter = find(key);
    if (iter == end()) return 0u;
    erase(iter);
    return 1u;
  }

  size_t count(const key_type& key) const {
    return (find(key) == end()) ? 0 : 1;
  }

  size_t size() const {
    if (size_ >= 0) {
      return static_cast<size_t>(size_);
    } else {
      return map_->size();
    }
  }

  bool empty() const {
    if (size_ >= 0) {
      return (size_ == 0);
    } else {
      return map_->empty();
    }
  }

  
  
  bool UsingFullMap() const {
    return size_ < 0;
  }

  inline NormalMap* map() {
    CHECK(UsingFullMap());
    return map_.get();
  }
  inline const NormalMap* map() const {
    CHECK(UsingFullMap());
    return map_.get();
  }

 private:
  int size_;  

  MapInit functor_;

  
  
  
  
  
  
  
  
  
  
  
  union {
    ManualConstructor<value_type> dummy_;
    ManualConstructor<value_type> array_[kArraySize];
    ManualConstructor<NormalMap> map_;
  };

  void ConvertToRealMap() {
    
    ManualConstructor<value_type> temp_array[kArraySize];

    for (int i = 0; i < kArraySize; i++) {
      temp_array[i].Init(*array_[i]);
      array_[i].Destroy();
    }

    
    size_ = -1;
    functor_(&map_);

    
    for (int i = 0; i < kArraySize; i++) {
      map_->insert(*temp_array[i]);
      temp_array[i].Destroy();
    }
  }

  
  void InitFrom(const SmallMap& src) {
    functor_ = src.functor_;
    size_ = src.size_;
    if (src.size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        array_[i].Init(*src.array_[i]);
      }
    } else {
      functor_(&map_);
      (*map_.get()) = (*src.map_.get());
    }
  }
  void Destroy() {
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        array_[i].Destroy();
      }
    } else {
      map_.Destroy();
    }
  }
};

template <typename NormalMap, int kArraySize, typename EqualKey,
          typename Functor>
inline bool SmallMap<NormalMap, kArraySize, EqualKey,
                     Functor>::iterator::operator==(
    const const_iterator& other) const {
  return other == *this;
}
template <typename NormalMap, int kArraySize, typename EqualKey,
          typename Functor>
inline bool SmallMap<NormalMap, kArraySize, EqualKey,
                     Functor>::iterator::operator!=(
    const const_iterator& other) const {
  return other != *this;
}

}  

#endif  
