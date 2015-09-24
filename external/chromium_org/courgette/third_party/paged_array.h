// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_BSDIFF_PAGED_ARRAY_H_
#define COURGETTE_BSDIFF_PAGED_ARRAY_H_

#include <new>

#include "base/basictypes.h"

namespace courgette {

template<typename T>
class PagedArray {
  enum {
    
    kLogPageSize = 18,
    kPageSize = 1 << kLogPageSize
  };

 public:
  PagedArray() : pages_(NULL), page_count_(0) {}

  ~PagedArray() { clear(); }

  T& operator[](size_t i) {
    size_t page = i >> kLogPageSize;
    size_t offset = i & (kPageSize - 1);
    
    
    return pages_[page][offset];
  }

  
  
  bool Allocate(size_t size) {
    clear();
    size_t pages_needed = (size + kPageSize - 1) >> kLogPageSize;
    pages_ = new(std::nothrow) T*[pages_needed];
    if (pages_ == NULL)
      return false;

    for (page_count_ = 0; page_count_ < pages_needed; ++page_count_) {
      T* block = new(std::nothrow) T[kPageSize];
      if (block == NULL) {
        clear();
        return false;
      }
      pages_[page_count_] = block;
    }
    return true;
  }

  
  void clear() {
    if (pages_ != NULL) {
      while (page_count_ != 0) {
        --page_count_;
        delete[] pages_[page_count_];
      }
      delete[] pages_;
      pages_ = NULL;
    }
  }

 private:
  T** pages_;
  size_t page_count_;

  DISALLOW_COPY_AND_ASSIGN(PagedArray);
};
}  
#endif  
