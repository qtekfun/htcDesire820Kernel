// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SANDBOX_LINUX_SANDBOX_BPF_BASE_POLICY_LINUX_H_
#define CONTENT_COMMON_SANDBOX_LINUX_SANDBOX_BPF_BASE_POLICY_LINUX_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "sandbox/linux/seccomp-bpf-helpers/baseline_policy.h"
#include "sandbox/linux/seccomp-bpf/sandbox_bpf_policy.h"

using sandbox::ErrorCode;
using sandbox::SandboxBPF;

namespace content {

class SandboxBPFBasePolicy : public sandbox::SandboxBPFPolicy {
 public:
  SandboxBPFBasePolicy();
  virtual ~SandboxBPFBasePolicy();

  virtual ErrorCode EvaluateSyscall(SandboxBPF* sandbox_compiler,
                                    int system_call_number) const OVERRIDE;

  
  
  
  
  
  
  virtual bool PreSandboxHook();

  
  static int GetFSDeniedErrno();

 private:
  
  scoped_ptr<sandbox::BaselinePolicy> baseline_policy_;
  DISALLOW_COPY_AND_ASSIGN(SandboxBPFBasePolicy);
};

}  

#endif  
