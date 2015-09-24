// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEMORY_WATCHER_MEMORY_HOOK_
#define MEMORY_WATCHER_MEMORY_HOOK_

#include "base/logging.h"

template <class T>
class PrivateHookAllocator {
 public:
  
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef T         value_type;

  PrivateHookAllocator() {}

  
  pointer allocate(size_type n, const void * = 0) {
    return reinterpret_cast<T*>(MemoryHook::Alloc(n * sizeof(T)));
  }

  
  void deallocate(void* p, size_type) {
    if (p)
      MemoryHook::Free(p);
  }

  
  void construct(pointer p, const T& val) {
    new (reinterpret_cast<T*>(p))T(val);
  }

  
  void destroy(pointer p) { p->~T(); }

  size_type max_size() const { return size_t(-1); }

  template <class U>
  struct rebind { typedef PrivateHookAllocator<U> other; };

  template <class U>
  PrivateHookAllocator(const PrivateHookAllocator<U>&) {}
};

template<class T, class U> inline
bool operator==(const PrivateHookAllocator<T>&,
                const PrivateHookAllocator<U>&) {
  return (true);
}

template<class T, class U> inline
bool operator!=(const PrivateHookAllocator<T>& left,
                const PrivateHookAllocator<U>& right) {
  return (!(left == right));
}


class MemoryObserver {
 public:
  virtual ~MemoryObserver() {}

  
  virtual void OnTrack(HANDLE heap, int32 id, int32 size) = 0;

  
  virtual void OnUntrack(HANDLE heap, int32 id, int32 size) = 0;
};

class MemoryHook : MemoryObserver {
 public:
  
  
  
  static bool Initialize();

  
  
  static bool hooked() { return hooked_ != NULL; }

  
  
  
  static bool RegisterWatcher(MemoryObserver* watcher);

  
  
  static bool UnregisterWatcher(MemoryObserver* watcher);

  
  
  static void* Alloc(size_t size) {
    DCHECK(global_hook_ && global_hook_->heap_);
    return HeapAlloc(global_hook_->heap_, 0, size);
  }
  static void Free(void* ptr) {
    DCHECK(global_hook_ && global_hook_->heap_);
    HeapFree(global_hook_->heap_, 0, ptr);
  }

  
  
  static MemoryHook* hook() { return global_hook_; }

  
  virtual void OnTrack(HANDLE hHeap, int32 id, int32 size);
  virtual void OnUntrack(HANDLE hHeap, int32 id, int32 size);

 private:
  MemoryHook();
  ~MemoryHook();

  
  
  
  static bool Hook();

  
  static bool Unhook();

  
  bool CreateHeap();

  
  bool CloseHeap();

  MemoryObserver* watcher_;
  HANDLE heap_;   
  static bool hooked_;
  static MemoryHook* global_hook_;
};

#endif  
