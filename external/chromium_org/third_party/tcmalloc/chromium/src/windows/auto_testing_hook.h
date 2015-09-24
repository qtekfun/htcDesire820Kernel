// Copyright (c) 2010 The Chromium Authors. All rights reserved.
//    * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef CEEE_TESTING_SIDESTEP_AUTO_TESTING_HOOK_H_
#define CEEE_TESTING_SIDESTEP_AUTO_TESTING_HOOK_H_

#include "base/basictypes.h"
#include "base/logging.h"
#include "preamble_patcher.h"

#define SIDESTEP_CHK(x)  CHECK(x)
#define SIDESTEP_EXPECT_TRUE(x)  SIDESTEP_CHK(x)

namespace sidestep {

class AutoTestingHookBase {
 public:
  virtual ~AutoTestingHookBase() {}
};

typedef const AutoTestingHookBase& AutoTestingHook;

class AutoTestingHookHolder {
 public:
  explicit AutoTestingHookHolder(AutoTestingHookBase* hook) : hook_(hook) {}
  ~AutoTestingHookHolder() { delete hook_; }
 private:
  AutoTestingHookHolder() {}  
  AutoTestingHookBase* hook_;
};

template <typename T>
class AutoTestingHookImpl : public AutoTestingHookBase {
 public:
  static AutoTestingHookImpl<T> MakeTestingHook(T target_function,
                                                T replacement_function,
                                                bool do_it) {
    return AutoTestingHookImpl<T>(target_function, replacement_function, do_it);
  }

  static AutoTestingHookImpl<T>* MakeTestingHookHolder(T target_function,
                                                       T replacement_function,
                                                       bool do_it) {
    return new AutoTestingHookImpl<T>(target_function,
                                      replacement_function, do_it);
  }

  ~AutoTestingHookImpl() {
    if (did_it_) {
      SIDESTEP_CHK(SIDESTEP_SUCCESS == PreamblePatcher::Unpatch(
          (void*)target_function_, (void*)replacement_function_,
          (void*)original_function_));
    }
  }

  
  
  
  T original_function() {
    return original_function_;
  }

 private:
  AutoTestingHookImpl(T target_function, T replacement_function, bool do_it)
      : target_function_(target_function),
        original_function_(NULL),
        replacement_function_(replacement_function),
        did_it_(do_it) {
    if (do_it) {
      SIDESTEP_CHK(SIDESTEP_SUCCESS == PreamblePatcher::Patch(target_function,
                                                     replacement_function,
                                                     &original_function_));
    }
  }

  T target_function_;  
  T original_function_;  
  T replacement_function_;  
  bool did_it_;  
};

template <typename T>
inline AutoTestingHookImpl<T> MakeTestingHook(T target,
                                              T replacement,
                                              bool do_it) {
  return AutoTestingHookImpl<T>::MakeTestingHook(target, replacement, do_it);
}

template <typename T>
inline AutoTestingHookImpl<T> MakeTestingHook(T target, T replacement) {
  return AutoTestingHookImpl<T>::MakeTestingHook(target, replacement, true);
}

template <typename T>
inline AutoTestingHookImpl<T>* MakeTestingHookHolder(T target, T replacement) {
  return AutoTestingHookImpl<T>::MakeTestingHookHolder(target, replacement,
                                                       true);
}

};  

#endif  
