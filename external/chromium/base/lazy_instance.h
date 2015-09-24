// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_LAZY_INSTANCE_H_
#define BASE_LAZY_INSTANCE_H_
#pragma once

#include <new>  

#include "base/atomicops.h"
#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/third_party/dynamic_annotations/dynamic_annotations.h"
#include "base/threading/thread_restrictions.h"

namespace base {

template <typename Type>
struct DefaultLazyInstanceTraits {
  static const bool kAllowedToAccessOnNonjoinableThread = false;

  static Type* New(void* instance) {
    
    
    return new (instance) Type();
  }
  static void Delete(void* instance) {
    
    reinterpret_cast<Type*>(instance)->~Type();
  }
};

template <typename Type>
struct LeakyLazyInstanceTraits {
  static const bool kAllowedToAccessOnNonjoinableThread = true;

  static Type* New(void* instance) {
    return DefaultLazyInstanceTraits<Type>::New(instance);
  }
  
  
  
  
  
  static void (*Delete)(void* instance);
};

template <typename Type>
void (*LeakyLazyInstanceTraits<Type>::Delete)(void* instance) = NULL;

class BASE_API LazyInstanceHelper {
 protected:
  enum {
    STATE_EMPTY    = 0,
    STATE_CREATING = 1,
    STATE_CREATED  = 2
  };

  explicit LazyInstanceHelper(LinkerInitialized ) {}
  
  

  
  
  
  bool NeedsInstance();

  
  
  void CompleteInstance(void* instance, void (*dtor)(void*));

  base::subtle::Atomic32 state_;

 private:
  DISALLOW_COPY_AND_ASSIGN(LazyInstanceHelper);
};

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 2))
#define LAZY_ALIGN(T) __attribute__((aligned(__alignof__(T))))
#else
#define LAZY_ALIGN(T)
#endif

template <typename Type, typename Traits = DefaultLazyInstanceTraits<Type> >
class LazyInstance : public LazyInstanceHelper {
 public:
  explicit LazyInstance(LinkerInitialized x) : LazyInstanceHelper(x) { }
  
  

  Type& Get() {
    return *Pointer();
  }

  Type* Pointer() {
    if (!Traits::kAllowedToAccessOnNonjoinableThread)
      base::ThreadRestrictions::AssertSingletonAllowed();

    
    if ((base::subtle::NoBarrier_Load(&state_) != STATE_CREATED) &&
        NeedsInstance()) {
      
      instance_ = Traits::New(buf_);
      
      void (*dtor)(void*) = Traits::Delete;
      CompleteInstance(this, (dtor == NULL) ? NULL : OnExit);
    }

    
    
    
    
    
    ANNOTATE_HAPPENS_AFTER(&state_);
    return instance_;
  }

  bool operator==(Type* p) {
    switch (base::subtle::NoBarrier_Load(&state_)) {
      case STATE_EMPTY:
        return p == NULL;
      case STATE_CREATING:
        return static_cast<int8*>(static_cast<void*>(p)) == buf_;
      case STATE_CREATED:
        return p == instance_;
      default:
        return false;
    }
  }

 private:
  
  
  
  static void OnExit(void* lazy_instance) {
    LazyInstance<Type, Traits>* me =
        reinterpret_cast<LazyInstance<Type, Traits>*>(lazy_instance);
    Traits::Delete(me->instance_);
    me->instance_ = NULL;
    base::subtle::Release_Store(&me->state_, STATE_EMPTY);
  }

  
  int8 buf_[sizeof(Type)] LAZY_ALIGN(Type);
  Type *instance_;

  DISALLOW_COPY_AND_ASSIGN(LazyInstance);
};

#undef LAZY_ALIGN

}  

#endif  
