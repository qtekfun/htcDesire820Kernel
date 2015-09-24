// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_PAGE_HEAP_ALLOCATOR_H_
#define TCMALLOC_PAGE_HEAP_ALLOCATOR_H_

#include <stddef.h>                     

#include "common.h"            
#include "free_list.h"          
#include "internal_logging.h"  
#include "system-alloc.h"      

namespace tcmalloc {

template <class T>
class PageHeapAllocator {
 public:
  
  
  
  void Init() {
    ASSERT(sizeof(T) <= kAllocIncrement);
    inuse_ = 0;
    free_area_ = NULL;
    free_avail_ = 0;
    free_list_ = NULL;
    
    Delete(New());
  }

  T* New() {
    
    void* result;
    if (free_list_ != NULL) {
      result = FL_Pop(&free_list_);
    } else {
      if (free_avail_ < sizeof(T)) {
        
        
        free_area_ = reinterpret_cast<char*>(MetaDataAlloc(kAllocIncrement));
        if (free_area_ == NULL) {
          Log(kCrash, __FILE__, __LINE__,
              "FATAL ERROR: Out of memory trying to allocate internal "
              "tcmalloc data (bytes, object-size)",
              kAllocIncrement, sizeof(T));
        }

        
        
        
        
        size_t guard_size = TCMalloc_SystemAddGuard(free_area_,
                                                    kAllocIncrement);
        free_area_ += guard_size;
        free_avail_ = kAllocIncrement - guard_size;
        if (free_avail_ < sizeof(T)) {
          Log(kCrash, __FILE__, __LINE__,
              "FATAL ERROR: Insufficient memory to guard internal tcmalloc "
              "data (%d bytes, object-size %d, guard-size %d)\n",
              kAllocIncrement, static_cast<int>(sizeof(T)), guard_size);
        }
      }
      result = free_area_;
      free_area_ += sizeof(T);
      free_avail_ -= sizeof(T);
    }
    inuse_++;
    return reinterpret_cast<T*>(result);
  }

  void Delete(T* p) {
    FL_Push(&free_list_, p);
    inuse_--;
  }

  int inuse() const { return inuse_; }

 private:
  
  static const int kAllocIncrement = 128 << 10;

  
  char* free_area_;
  size_t free_avail_;

  
  void* free_list_;

  
  int inuse_;
};

}  

#endif  
