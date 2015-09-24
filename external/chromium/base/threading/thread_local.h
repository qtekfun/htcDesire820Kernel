// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_THREADING_THREAD_LOCAL_H_
#define BASE_THREADING_THREAD_LOCAL_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"

#if defined(OS_POSIX)
#include <pthread.h>
#endif

namespace base {

namespace internal {

struct BASE_API ThreadLocalPlatform {
#if defined(OS_WIN)
  typedef unsigned long SlotType;
#elif defined(OS_POSIX)
  typedef pthread_key_t SlotType;
#endif

  static void AllocateSlot(SlotType& slot);
  static void FreeSlot(SlotType& slot);
  static void* GetValueFromSlot(SlotType& slot);
  static void SetValueInSlot(SlotType& slot, void* value);
};

}  

template <typename Type>
class ThreadLocalPointer {
 public:
  ThreadLocalPointer() : slot_() {
    internal::ThreadLocalPlatform::AllocateSlot(slot_);
  }

  ~ThreadLocalPointer() {
    internal::ThreadLocalPlatform::FreeSlot(slot_);
  }

  Type* Get() {
    return static_cast<Type*>(
        internal::ThreadLocalPlatform::GetValueFromSlot(slot_));
  }

  void Set(Type* ptr) {
    internal::ThreadLocalPlatform::SetValueInSlot(slot_, ptr);
  }

 private:
  typedef internal::ThreadLocalPlatform::SlotType SlotType;

  SlotType slot_;

  DISALLOW_COPY_AND_ASSIGN(ThreadLocalPointer<Type>);
};

class ThreadLocalBoolean {
 public:
  ThreadLocalBoolean() { }
  ~ThreadLocalBoolean() { }

  bool Get() {
    return tlp_.Get() != NULL;
  }

  void Set(bool val) {
    tlp_.Set(reinterpret_cast<void*>(val ? 1 : 0));
  }

 private:
  ThreadLocalPointer<void> tlp_;

  DISALLOW_COPY_AND_ASSIGN(ThreadLocalBoolean);
};

}  

#endif  
