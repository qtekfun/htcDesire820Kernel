// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef _MALLOC_HOOK_INL_H_
#define _MALLOC_HOOK_INL_H_

#include <stddef.h>
#include <sys/types.h>
#include "base/atomicops.h"
#include "base/basictypes.h"
#include <gperftools/malloc_hook.h>

namespace base { namespace internal {

template<typename PtrT>
class AtomicPtr {
 public:
  COMPILE_ASSERT(sizeof(PtrT) <= sizeof(AtomicWord),
                 PtrT_should_fit_in_AtomicWord);

  PtrT Get() const {
    
    
    
    
    
    return reinterpret_cast<PtrT>(static_cast<AtomicWord>(
      base::subtle::Acquire_Load(&data_)));
  }

  
  
  
  PtrT Exchange(PtrT new_val);

  
  
  
  
  
  
  PtrT CompareAndSwap(PtrT old_val, PtrT new_val);

  
  
  AtomicWord data_;
};

extern AtomicPtr<MallocHook::NewHook>     new_hook_;
extern AtomicPtr<MallocHook::DeleteHook>  delete_hook_;
extern AtomicPtr<MallocHook::PreMmapHook> premmap_hook_;
extern AtomicPtr<MallocHook::MmapHook>    mmap_hook_;
extern AtomicPtr<MallocHook::MunmapHook>  munmap_hook_;
extern AtomicPtr<MallocHook::MremapHook>  mremap_hook_;
extern AtomicPtr<MallocHook::PreSbrkHook> presbrk_hook_;
extern AtomicPtr<MallocHook::SbrkHook>    sbrk_hook_;

static const int kHookListMaxValues = 7;

template <typename T>
struct PERFTOOLS_DLL_DECL HookList {
  COMPILE_ASSERT(sizeof(T) <= sizeof(AtomicWord), T_should_fit_in_AtomicWord);

  
  
  
  bool Add(T value);

  
  
  
  bool Remove(T value);

  
  
  
  int Traverse(T* output_array, int n) const;

  
  bool empty() const {
    return base::subtle::Acquire_Load(&priv_end) == 0;
  }

  
  
  

  
  
  
  AtomicWord priv_end;
  AtomicWord priv_data[kHookListMaxValues];
};

extern HookList<MallocHook::NewHook> new_hooks_;
extern HookList<MallocHook::DeleteHook> delete_hooks_;
extern HookList<MallocHook::PreMmapHook> premmap_hooks_;
extern HookList<MallocHook::MmapHook> mmap_hooks_;
extern HookList<MallocHook::MmapReplacement> mmap_replacement_;
extern HookList<MallocHook::MunmapHook> munmap_hooks_;
extern HookList<MallocHook::MunmapReplacement> munmap_replacement_;
extern HookList<MallocHook::MremapHook> mremap_hooks_;
extern HookList<MallocHook::PreSbrkHook> presbrk_hooks_;
extern HookList<MallocHook::SbrkHook> sbrk_hooks_;

} }  

inline MallocHook::NewHook MallocHook::GetNewHook() {
  return base::internal::new_hook_.Get();
}

inline void MallocHook::InvokeNewHook(const void* p, size_t s) {
  if (!base::internal::new_hooks_.empty()) {
    InvokeNewHookSlow(p, s);
  }
  
  MallocHook::NewHook hook = MallocHook::GetNewHook();
  if (hook != NULL) (*hook)(p, s);
  
}

inline MallocHook::DeleteHook MallocHook::GetDeleteHook() {
  return base::internal::delete_hook_.Get();
}

inline void MallocHook::InvokeDeleteHook(const void* p) {
  if (!base::internal::delete_hooks_.empty()) {
    InvokeDeleteHookSlow(p);
  }
  
  MallocHook::DeleteHook hook = MallocHook::GetDeleteHook();
  if (hook != NULL) (*hook)(p);
  
}

inline MallocHook::PreMmapHook MallocHook::GetPreMmapHook() {
  return base::internal::premmap_hook_.Get();
}

inline void MallocHook::InvokePreMmapHook(const void* start,
                                          size_t size,
                                          int protection,
                                          int flags,
                                          int fd,
                                          off_t offset) {
  if (!base::internal::premmap_hooks_.empty()) {
    InvokePreMmapHookSlow(start, size, protection, flags, fd, offset);
  }
  
  MallocHook::PreMmapHook hook = MallocHook::GetPreMmapHook();
  if (hook != NULL) (*hook)(start, size,
                            protection, flags,
                            fd, offset);
  
}

inline MallocHook::MmapHook MallocHook::GetMmapHook() {
  return base::internal::mmap_hook_.Get();
}

inline void MallocHook::InvokeMmapHook(const void* result,
                                       const void* start,
                                       size_t size,
                                       int protection,
                                       int flags,
                                       int fd,
                                       off_t offset) {
  if (!base::internal::mmap_hooks_.empty()) {
    InvokeMmapHookSlow(result, start, size, protection, flags, fd, offset);
  }
  
  MallocHook::MmapHook hook = MallocHook::GetMmapHook();
  if (hook != NULL) (*hook)(result,
                            start, size,
                            protection, flags,
                            fd, offset);
  
}

inline bool MallocHook::InvokeMmapReplacement(const void* start,
                                              size_t size,
                                              int protection,
                                              int flags,
                                              int fd,
                                              off_t offset,
                                              void** result) {
  if (!base::internal::mmap_replacement_.empty()) {
    return InvokeMmapReplacementSlow(start, size,
                                     protection, flags,
                                     fd, offset,
                                     result);
  }
  return false;
}

inline MallocHook::MunmapHook MallocHook::GetMunmapHook() {
  return base::internal::munmap_hook_.Get();
}

inline void MallocHook::InvokeMunmapHook(const void* p, size_t size) {
  if (!base::internal::munmap_hooks_.empty()) {
    InvokeMunmapHookSlow(p, size);
  }
  
  MallocHook::MunmapHook hook = MallocHook::GetMunmapHook();
  if (hook != NULL) (*hook)(p, size);
  
}

inline bool MallocHook::InvokeMunmapReplacement(
    const void* p, size_t size, int* result) {
  if (!base::internal::mmap_replacement_.empty()) {
    return InvokeMunmapReplacementSlow(p, size, result);
  }
  return false;
}

inline MallocHook::MremapHook MallocHook::GetMremapHook() {
  return base::internal::mremap_hook_.Get();
}

inline void MallocHook::InvokeMremapHook(const void* result,
                                         const void* old_addr,
                                         size_t old_size,
                                         size_t new_size,
                                         int flags,
                                         const void* new_addr) {
  if (!base::internal::mremap_hooks_.empty()) {
    InvokeMremapHookSlow(result, old_addr, old_size, new_size, flags, new_addr);
  }
  
  MallocHook::MremapHook hook = MallocHook::GetMremapHook();
  if (hook != NULL) (*hook)(result,
                            old_addr, old_size,
                            new_size, flags, new_addr);
  
}

inline MallocHook::PreSbrkHook MallocHook::GetPreSbrkHook() {
  return base::internal::presbrk_hook_.Get();
}

inline void MallocHook::InvokePreSbrkHook(ptrdiff_t increment) {
  if (!base::internal::presbrk_hooks_.empty() && increment != 0) {
    InvokePreSbrkHookSlow(increment);
  }
  
  MallocHook::PreSbrkHook hook = MallocHook::GetPreSbrkHook();
  if (hook != NULL && increment != 0) (*hook)(increment);
  
}

inline MallocHook::SbrkHook MallocHook::GetSbrkHook() {
  return base::internal::sbrk_hook_.Get();
}

inline void MallocHook::InvokeSbrkHook(const void* result,
                                       ptrdiff_t increment) {
  if (!base::internal::sbrk_hooks_.empty() && increment != 0) {
    InvokeSbrkHookSlow(result, increment);
  }
  
  MallocHook::SbrkHook hook = MallocHook::GetSbrkHook();
  if (hook != NULL && increment != 0) (*hook)(result, increment);
  
}

#endif 
