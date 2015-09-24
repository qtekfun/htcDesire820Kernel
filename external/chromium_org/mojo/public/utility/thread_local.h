// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MOJO_PUBLIC_UTILITY_THREAD_LOCAL_H_
#define MOJO_PUBLIC_UTILITY_THREAD_LOCAL_H_

#include "build/build_config.h"

#if defined(OS_POSIX)
#include <pthread.h>
#endif

#include "mojo/public/system/macros.h"

namespace mojo {
namespace utility {
namespace internal {

struct ThreadLocalPlatform {
#if defined(OS_WIN)
  typedef unsigned long SlotType;
#elif defined(OS_POSIX)
  typedef pthread_key_t SlotType;
#endif

  static void AllocateSlot(SlotType* slot);
  static void FreeSlot(SlotType slot);
  static void* GetValueFromSlot(SlotType slot);
  static void SetValueInSlot(SlotType slot, void* value);
};

}  

template <typename Type>
class ThreadLocalPointer {
 public:
  ThreadLocalPointer() : slot_() {
    internal::ThreadLocalPlatform::AllocateSlot(&slot_);
  }

  ~ThreadLocalPointer() {
    internal::ThreadLocalPlatform::FreeSlot(slot_);
  }

  Type* Get() {
    return static_cast<Type*>(
        internal::ThreadLocalPlatform::GetValueFromSlot(slot_));
  }

  void Set(Type* ptr) {
    internal::ThreadLocalPlatform::SetValueInSlot(
        slot_, const_cast<void*>(static_cast<const void*>(ptr)));
  }

 private:
  typedef internal::ThreadLocalPlatform::SlotType SlotType;

  SlotType slot_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(ThreadLocalPointer<Type>);
};

class ThreadLocalBoolean {
 public:
  ThreadLocalBoolean() {}
  ~ThreadLocalBoolean() {}

  bool Get() {
    return tlp_.Get() != NULL;
  }

  void Set(bool val) {
    tlp_.Set(val ? this : NULL);
  }

 private:
  ThreadLocalPointer<void> tlp_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(ThreadLocalBoolean);
};

}  
}  

#endif  
