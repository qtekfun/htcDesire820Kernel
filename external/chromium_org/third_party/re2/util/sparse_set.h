// Copyright 2006 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.



#ifndef RE2_UTIL_SPARSE_SET_H__
#define RE2_UTIL_SPARSE_SET_H__

#include "util/util.h"

namespace re2 {

class SparseSet {
 public:
  SparseSet()
    : size_(0), max_size_(0), sparse_to_dense_(NULL), dense_(NULL), valgrind_(RunningOnValgrind()) {}

  SparseSet(int max_size) {
    max_size_ = max_size;
    sparse_to_dense_ = new int[max_size];
    dense_ = new int[max_size];
    valgrind_ = RunningOnValgrind();
    
    
    if (valgrind_) {
      for (int i = 0; i < max_size; i++) {
        dense_[i] = 0xababababU;
        sparse_to_dense_[i] = 0xababababU;
      }
    }
    size_ = 0;
  }

  ~SparseSet() {
    delete[] sparse_to_dense_;
    delete[] dense_;
  }

  typedef int* iterator;
  typedef const int* const_iterator;

  int size() const { return size_; }
  iterator begin() { return dense_; }
  iterator end() { return dense_ + size_; }
  const_iterator begin() const { return dense_; }
  const_iterator end() const { return dense_ + size_; }

  
  
  void resize(int new_max_size) {
    if (size_ > new_max_size)
      size_ = new_max_size;
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

      a = new int[new_max_size];
      if (dense_) {
        memmove(a, dense_, size_*sizeof a[0]);
        if (valgrind_) {
          for (int i = size_; i < new_max_size; i++)
            a[i] = 0xababababU;
        }
        delete[] dense_;
      }
      dense_ = a;
    }
    max_size_ = new_max_size;
  }

  
  
  int max_size() const { return max_size_; }

  
  void clear() { size_ = 0; }

  
  bool contains(int i) const {
    DCHECK_GE(i, 0);
    DCHECK_LT(i, max_size_);
    if (static_cast<uint>(i) >= max_size_) {
      return false;
    }
    
    return (uint)sparse_to_dense_[i] < (uint)size_ && 
      dense_[sparse_to_dense_[i]] == i;
  }

  
  void insert(int i) {
    if (!contains(i))
      insert_new(i);
  }

  
  
  void insert_new(int i) {
    if (static_cast<uint>(i) >= max_size_) {
      
      
      
      return;
    }
    DCHECK(!contains(i));
    DCHECK_LT(size_, max_size_);
    sparse_to_dense_[i] = size_;
    dense_[size_] = i;
    size_++;
  }

  
  
  
  
  static bool less(int a, int b) { return a < b; }

 private:
  int size_;
  int max_size_;
  int* sparse_to_dense_;
  int* dense_;
  bool valgrind_;

  DISALLOW_EVIL_CONSTRUCTORS(SparseSet);
};

}  

#endif  
