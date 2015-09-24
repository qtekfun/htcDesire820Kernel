// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_PAGEMAP_H_
#define TCMALLOC_PAGEMAP_H_

#include "config.h"

#include <stddef.h>                     
#include <string.h>                     
#if defined HAVE_STDINT_H
#include <stdint.h>
#elif defined HAVE_INTTYPES_H
#include <inttypes.h>
#else
#include <sys/types.h>
#endif
#ifdef WIN32
#include "common.h"
#endif

#include "internal_logging.h"  

template <int BITS>
class TCMalloc_PageMap1 {
 private:
  static const int LENGTH = 1 << BITS;

  void** array_;

 public:
  typedef uintptr_t Number;

  explicit TCMalloc_PageMap1(void* (*allocator)(size_t)) {
    array_ = reinterpret_cast<void**>((*allocator)(sizeof(void*) << BITS));
    memset(array_, 0, sizeof(void*) << BITS);
  }

  
  
  bool Ensure(Number x, size_t n) {
    
    
    
    
    return n <= LENGTH - x;   
  }

  void PreallocateMoreMemory() {}

  
  
  void* get(Number k) const {
    if ((k >> BITS) > 0) {
      return NULL;
    }
    return array_[k];
  }

  
  
  
  
  void set(Number k, void* v) {
    array_[k] = v;
  }

  
  
  void* Next(Number k) const {
    while (k < (1 << BITS)) {
      if (array_[k] != NULL) return array_[k];
      k++;
    }
    return NULL;
  }
};

#ifdef WIN32
template <int BITS>
class TCMalloc_PageMap1_LazyCommit {
 private:
  
  static const int LENGTH = 1 << BITS;

  
  
  
  void** array_;

  
  
  
  char committed_[sizeof(void*) << (BITS - kPageShift - 3)];

  
  
  size_t ContainingPage(size_t index) const {
    return (index * sizeof(*array_)) >> kPageShift;
  }

  
  bool IsCommitted(size_t page_num) const {
    return committed_[page_num >> 3] & (1 << (page_num & 0x7));
  }

  
  void SetCommitted(size_t page_num) {
    committed_[page_num >> 3] |= (1 << (page_num & 0x7));
  }

 public:
  typedef uintptr_t Number;

  explicit TCMalloc_PageMap1_LazyCommit(void* (*allocator)(size_t)) {
    
    
    
    size_t size = sizeof(*array_) << BITS;
    array_ = reinterpret_cast<void**>(VirtualAlloc(
        NULL, size, MEM_RESERVE, PAGE_READWRITE));
    tcmalloc::update_metadata_system_bytes(size);
    tcmalloc::update_metadata_unmapped_bytes(size);

    
    ASSERT(sizeof(committed_) * 8 == (LENGTH * sizeof(*array_)) >> kPageShift);
    
    memset(committed_, 0, sizeof(committed_));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Ensure(Number x, size_t n) {
    if (n > LENGTH - x)
      return false;  
    ASSERT(n > 0);

    
    
    
    
    
    
    
    size_t first_page = ContainingPage(x);
    size_t last_page = ContainingPage(x + n - 1);

    
    
    
    if (IsCommitted(first_page)) {
      if (first_page == last_page) return true;
      ++first_page;
      if (IsCommitted(first_page)) {
        if (first_page == last_page) return true;
        ++first_page;
      }
    }

    if (IsCommitted(last_page)) {
      if (first_page == last_page) return true;
      --last_page;
      if (IsCommitted(last_page)) {
        if (first_page == last_page) return true;
        --last_page;
      }
    }

    ASSERT(!IsCommitted(last_page));
    ASSERT(!IsCommitted(first_page));

    void* start = reinterpret_cast<char*>(array_) + (first_page << kPageShift);
    size_t length = (last_page - first_page + 1) << kPageShift;

#ifndef NDEBUG
    
    
    MEMORY_BASIC_INFORMATION info = {0};
    size_t result = VirtualQuery(start, &info, sizeof(info));
    ASSERT(result);
    ASSERT(0 == (info.State & MEM_COMMIT));  
    ASSERT(info.RegionSize >= length);       
#endif

    TCMalloc_SystemCommit(start, length);
    tcmalloc::update_metadata_unmapped_bytes(-length);

#ifndef NDEBUG
    result = VirtualQuery(start, &info, sizeof(info));
    ASSERT(result);
    ASSERT(0 != (info.State & MEM_COMMIT));  
    ASSERT(info.RegionSize >= length);       
#endif

    
    
    
    
    
    SetCommitted(first_page);
    if (first_page < last_page) {
      SetCommitted(last_page);
      SetCommitted(first_page + 1);  
      SetCommitted(last_page - 1);
    }

    return true;
  }

  
  
  
  void PreallocateMoreMemory() {}

  
  
  void* get(Number k) const {
    if ((k >> BITS) > 0) {
      return NULL;
    }
    return array_[k];
  }

  
  
  
  
  void set(Number k, void* v) {
    array_[k] = v;
  }
  
  
  void* Next(Number k) const {
    while (k < (1 << BITS)) {
      if (array_[k] != NULL) return array_[k];
      k++;
    }
    return NULL;
  }
};
#endif  


template <int BITS>
class TCMalloc_PageMap2 {
 private:
  
  static const int ROOT_BITS = 5;
  static const int ROOT_LENGTH = 1 << ROOT_BITS;

  static const int LEAF_BITS = BITS - ROOT_BITS;
  static const int LEAF_LENGTH = 1 << LEAF_BITS;

  
  struct Leaf {
    void* values[LEAF_LENGTH];
  };

  Leaf* root_[ROOT_LENGTH];             
  void* (*allocator_)(size_t);          

 public:
  typedef uintptr_t Number;

  explicit TCMalloc_PageMap2(void* (*allocator)(size_t)) {
    allocator_ = allocator;
    memset(root_, 0, sizeof(root_));
  }

  void* get(Number k) const {
    const Number i1 = k >> LEAF_BITS;
    const Number i2 = k & (LEAF_LENGTH-1);
    if ((k >> BITS) > 0 || root_[i1] == NULL) {
      return NULL;
    }
    return root_[i1]->values[i2];
  }

  void set(Number k, void* v) {
    ASSERT(k >> BITS == 0);
    const Number i1 = k >> LEAF_BITS;
    const Number i2 = k & (LEAF_LENGTH-1);
    root_[i1]->values[i2] = v;
  }

  bool Ensure(Number start, size_t n) {
    for (Number key = start; key <= start + n - 1; ) {
      const Number i1 = key >> LEAF_BITS;

      
      if (i1 >= ROOT_LENGTH)
        return false;

      
      if (root_[i1] == NULL) {
        Leaf* leaf = reinterpret_cast<Leaf*>((*allocator_)(sizeof(Leaf)));
        if (leaf == NULL) return false;
        memset(leaf, 0, sizeof(*leaf));
        root_[i1] = leaf;
      }

      
      key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
    }
    return true;
  }

  void PreallocateMoreMemory() {
    
    Ensure(0, 1 << BITS);
  }

  void* Next(Number k) const {
    while (k < (1 << BITS)) {
      const Number i1 = k >> LEAF_BITS;
      Leaf* leaf = root_[i1];
      if (leaf != NULL) {
        
        for (Number i2 = k & (LEAF_LENGTH - 1); i2 < LEAF_LENGTH; i2++) {
          if (leaf->values[i2] != NULL) {
            return leaf->values[i2];
          }
        }
      }
      
      k = (i1 + 1) << LEAF_BITS;
    }
    return NULL;
  }
};

template <int BITS>
class TCMalloc_PageMap3 {
 private:
  
  static const int INTERIOR_BITS = (BITS + 2) / 3; 
  static const int INTERIOR_LENGTH = 1 << INTERIOR_BITS;

  
  static const int LEAF_BITS = BITS - 2*INTERIOR_BITS;
  static const int LEAF_LENGTH = 1 << LEAF_BITS;

  
  struct Node {
    Node* ptrs[INTERIOR_LENGTH];
  };

  
  struct Leaf {
    void* values[LEAF_LENGTH];
  };

  Node* root_;                          
  void* (*allocator_)(size_t);          

  Node* NewNode() {
    Node* result = reinterpret_cast<Node*>((*allocator_)(sizeof(Node)));
    if (result != NULL) {
      memset(result, 0, sizeof(*result));
    }
    return result;
  }

 public:
  typedef uintptr_t Number;

  explicit TCMalloc_PageMap3(void* (*allocator)(size_t)) {
    allocator_ = allocator;
    root_ = NewNode();
  }

  void* get(Number k) const {
    const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
    const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH-1);
    const Number i3 = k & (LEAF_LENGTH-1);
    if ((k >> BITS) > 0 ||
        root_->ptrs[i1] == NULL || root_->ptrs[i1]->ptrs[i2] == NULL) {
      return NULL;
    }
    return reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2])->values[i3];
  }

  void set(Number k, void* v) {
    ASSERT(k >> BITS == 0);
    const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
    const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH-1);
    const Number i3 = k & (LEAF_LENGTH-1);
    reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2])->values[i3] = v;
  }

  bool Ensure(Number start, size_t n) {
    for (Number key = start; key <= start + n - 1; ) {
      const Number i1 = key >> (LEAF_BITS + INTERIOR_BITS);
      const Number i2 = (key >> LEAF_BITS) & (INTERIOR_LENGTH-1);

      
      if (i1 >= INTERIOR_LENGTH || i2 >= INTERIOR_LENGTH)
        return false;

      
      if (root_->ptrs[i1] == NULL) {
        Node* n = NewNode();
        if (n == NULL) return false;
        root_->ptrs[i1] = n;
      }

      
      if (root_->ptrs[i1]->ptrs[i2] == NULL) {
        Leaf* leaf = reinterpret_cast<Leaf*>((*allocator_)(sizeof(Leaf)));
        if (leaf == NULL) return false;
        memset(leaf, 0, sizeof(*leaf));
        root_->ptrs[i1]->ptrs[i2] = reinterpret_cast<Node*>(leaf);
      }

      
      key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
    }
    return true;
  }

  void PreallocateMoreMemory() {
  }

  void* Next(Number k) const {
    while (k < (Number(1) << BITS)) {
      const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
      const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH-1);
      if (root_->ptrs[i1] == NULL) {
        
        k = (i1 + 1) << (LEAF_BITS + INTERIOR_BITS);
      } else {
        Leaf* leaf = reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2]);
        if (leaf != NULL) {
          for (Number i3 = (k & (LEAF_LENGTH-1)); i3 < LEAF_LENGTH; i3++) {
            if (leaf->values[i3] != NULL) {
              return leaf->values[i3];
            }
          }
        }
        
        k = ((k >> LEAF_BITS) + 1) << LEAF_BITS;
      }
    }
    return NULL;
  }
};

#endif  
