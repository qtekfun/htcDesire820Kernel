// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_HELPERS_BASELINE_POLICY_H_
#define SANDBOX_LINUX_SECCOMP_BPF_HELPERS_BASELINE_POLICY_H_

#include "sandbox/linux/seccomp-bpf/errorcode.h"
#include "sandbox/linux/seccomp-bpf/sandbox_bpf_policy.h"

namespace sandbox {

class SandboxBPF;
class SandboxBPFPolicy;

class BaselinePolicy : public SandboxBPFPolicy {
 public:
  BaselinePolicy();
  
  
  explicit BaselinePolicy(int fs_denied_errno);
  virtual ~BaselinePolicy();

  virtual ErrorCode EvaluateSyscall(SandboxBPF* sandbox_compiler,
                                    int system_call_number) const OVERRIDE;

 private:
  int fs_denied_errno_;
  DISALLOW_COPY_AND_ASSIGN(BaselinePolicy);
};

}  

#endif  
