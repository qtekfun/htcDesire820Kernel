// Copyright 2006 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.



#ifndef RE2_UTIL_SPARSE_ARRAY_H__
#define RE2_UTIL_SPARSE_ARRAY_H__

#include "util/util.h"

namespace re2 {

template<typename Value>
class SparseArray {
 public:
  SparseArray();
  SparseArray(int max_size);
  ~SparseArray();

  
  class IndexValue;

  typedef IndexValue value_type;
  typedef typename vector<IndexValue>::iterator iterator;
  typedef typename vector<IndexValue>::const_iterator const_iterator;

  inline const IndexValue& iv(int i) const;

  
  int size() const {
    return size_;
  }

  
  iterator begin() {
    return dense_.begin();
  }
  iterator end() {
    return dense_.begin() + size_;
  }

  const_iterator begin() const {
    return dense_.begin();
  }
  const_iterator end() const {
    return dense_.begin() + size_;
  }

  
  
  void resize(int max_size);

  
  
  int max_size() const {
    return max_size_;
  }

  
  void clear() {
    size_ = 0;
  }

  
  inline bool has_index(int i) const;

  
  
  
  
  static bool less(const IndexValue& a, const IndexValue& b);

 public:
  
  inline iterator set(int i, Value v);

  pair<iterator, bool> insert(const value_type& new_value);

  
  
  inline Value get(int i, Value defaultv) const;

  iterator find(int i);

  const_iterator find(int i) const;

  
  
  inline iterator set_existing(int i, Value v);

  
  
  inline iterator set_new(int i, Value v);

  
  
  inline Value get_existing(int i) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  inline void erase(int i);

  
  
  inline void erase_existing(int i);

 private:
  
  
  
  
  
  inline void create_index(int i);

  
  
  
  inline void DebugCheckInvariants() const;

  int size_;
  int max_size_;
  int* sparse_to_dense_;
  vector<IndexValue> dense_;
  bool valgrind_;

  DISALLOW_EVIL_CONSTRUCTORS(SparseArray);
};

template<typename Value>
SparseArray<Value>::SparseArray()
    : size_(0), max_size_(0), sparse_to_dense_(NULL), dense_(), valgrind_(RunningOnValgrind()) {}

template<typename Value>
class SparseArray<Value>::IndexValue {
  friend class SparseArray;
 public:
  typedef int first_type;
  typedef Value second_type;

  IndexValue() {}
  IndexValue(int index, const Value& value) : second(value), index_(index) {}

  int index() const { return index_; }
  Value value() const { return second; }

  
  
  Value second;

 private:
  int index_;
};

template<typename Value>
const typename SparseArray<Value>::IndexValue&
SparseArray<Value>::iv(int i) const {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, size_);
  return dense_[i];
}

template<typename Value>
void SparseArray<Value>::resize(int new_max_size) {
  DebugCheckInvariants();
  if (new_max_size > max_size_) {
    int* a = new int[new_max_size];
    if (sparse_to_dense_) {
      memmove(a, sparse_to_dense_, max_size_*sizeof a[0]);
      
      if (valgrind_) {
        for (int i = max_size_; i < new_max_size; i++)
          a[i] = 0xababababU;
      }
      delete[] sparse_to_dense_;
    }
    sparse_to_dense_ = a;

    dense_.resize(new_max_size);
  }
  max_size_ = new_max_size;
  if (size_ > max_size_)
    size_ = max_size_;
  DebugCheckInvariants();
}

template<typename Value>
bool SparseArray<Value>::has_index(int i) const {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, max_size_);
  if (static_cast<uint>(i) >= max_size_) {
    return false;
  }
  
  return (uint)sparse_to_dense_[i] < (uint)size_ && 
    dense_[sparse_to_dense_[i]].index_ == i;
}

template<typename Value>
typename SparseArray<Value>::iterator SparseArray<Value>::set(int i, Value v) {
  DebugCheckInvariants();
  if (static_cast<uint>(i) >= max_size_) {
    
    
    
    return begin();
  }
  if (!has_index(i))
    create_index(i);
  return set_existing(i, v);
}

template<typename Value>
pair<typename SparseArray<Value>::iterator, bool> SparseArray<Value>::insert(
    const value_type& new_value) {
  DebugCheckInvariants();
  pair<typename SparseArray<Value>::iterator, bool> p;
  if (has_index(new_value.index_)) {
    p = make_pair(dense_.begin() + sparse_to_dense_[new_value.index_], false);
  } else {
    p = make_pair(set_new(new_value.index_, new_value.second), true);
  }
  DebugCheckInvariants();
  return p;
}

template<typename Value>
Value SparseArray<Value>::get(int i, Value defaultv) const {
  if (!has_index(i))
    return defaultv;
  return get_existing(i);
}

template<typename Value>
typename SparseArray<Value>::iterator SparseArray<Value>::find(int i) {
  if (has_index(i))
    return dense_.begin() + sparse_to_dense_[i];
  return end();
}

template<typename Value>
typename SparseArray<Value>::const_iterator
SparseArray<Value>::find(int i) const {
  if (has_index(i)) {
    return dense_.begin() + sparse_to_dense_[i];
  }
  return end();
}

template<typename Value>
typename SparseArray<Value>::iterator
SparseArray<Value>::set_existing(int i, Value v) {
  DebugCheckInvariants();
  DCHECK(has_index(i));
  dense_[sparse_to_dense_[i]].second = v;
  DebugCheckInvariants();
  return dense_.begin() + sparse_to_dense_[i];
}

template<typename Value>
typename SparseArray<Value>::iterator
SparseArray<Value>::set_new(int i, Value v) {
  DebugCheckInvariants();
  if (static_cast<uint>(i) >= max_size_) {
    
    
    
    return begin();
  }
  DCHECK(!has_index(i));
  create_index(i);
  return set_existing(i, v);
}

template<typename Value>
Value SparseArray<Value>::get_existing(int i) const {
  DCHECK(has_index(i));
  return dense_[sparse_to_dense_[i]].second;
}

template<typename Value>
void SparseArray<Value>::erase(int i) {
  DebugCheckInvariants();
  if (has_index(i))
    erase_existing(i);
  DebugCheckInvariants();
}

template<typename Value>
void SparseArray<Value>::erase_existing(int i) {
  DebugCheckInvariants();
  DCHECK(has_index(i));
  int di = sparse_to_dense_[i];
  if (di < size_ - 1) {
    dense_[di] = dense_[size_ - 1];
    sparse_to_dense_[dense_[di].index_] = di;
  }
  size_--;
  DebugCheckInvariants();
}

template<typename Value>
void SparseArray<Value>::create_index(int i) {
  DCHECK(!has_index(i));
  DCHECK_LT(size_, max_size_);
  sparse_to_dense_[i] = size_;
  dense_[size_].index_ = i;
  size_++;
}

template<typename Value> SparseArray<Value>::SparseArray(int max_size) {
  max_size_ = max_size;
  sparse_to_dense_ = new int[max_size];
  valgrind_ = RunningOnValgrind();
  dense_.resize(max_size);
  
  if (valgrind_) {
    for (int i = 0; i < max_size; i++) {
      sparse_to_dense_[i] = 0xababababU;
      dense_[i].index_ = 0xababababU;
    }
  }
  size_ = 0;
  DebugCheckInvariants();
}

template<typename Value> SparseArray<Value>::~SparseArray() {
  DebugCheckInvariants();
  delete[] sparse_to_dense_;
}

template<typename Value> void SparseArray<Value>::DebugCheckInvariants() const {
  DCHECK_LE(0, size_);
  DCHECK_LE(size_, max_size_);
  DCHECK(size_ == 0 || sparse_to_dense_ != NULL);
}

template<typename Value> bool SparseArray<Value>::less(const IndexValue& a,
                                                       const IndexValue& b) {
  return a.index_ < b.index_;
}

}  

#endif  
