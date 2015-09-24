// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_SANDBOX_BPF_POLICY_H_
#define SANDBOX_LINUX_SECCOMP_BPF_SANDBOX_BPF_POLICY_H_

#include "base/basictypes.h"

namespace sandbox {

class ErrorCode;
class SandboxBPF;

class SandboxBPFPolicy {
 public:
  SandboxBPFPolicy() {}
  virtual ~SandboxBPFPolicy() {}

  
  
  
  
  
  virtual ErrorCode EvaluateSyscall(SandboxBPF* sandbox_compiler,
                                    int system_call_number) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SandboxBPFPolicy);
};

}  

#endif  
