// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_SYSCALL_ITERATOR_H__
#define SANDBOX_LINUX_SECCOMP_BPF_SYSCALL_ITERATOR_H__

#include <stdint.h>

#include "base/basictypes.h"

namespace sandbox {

class SyscallIterator {
 public:
  explicit SyscallIterator(bool invalid_only)
      : invalid_only_(invalid_only), done_(false), num_(0) {}

  bool Done() const { return done_; }
  uint32_t Next();
  static bool IsValid(uint32_t num);

 private:
  static bool IsArmPrivate(uint32_t num);

  bool invalid_only_;
  bool done_;
  uint32_t num_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SyscallIterator);
};

}  

#endif  
