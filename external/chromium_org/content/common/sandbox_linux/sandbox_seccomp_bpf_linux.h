// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SANDBOX_LINUX_SANDBOX_SECCOMP_BPF_LINUX_H_
#define CONTENT_COMMON_SANDBOX_LINUX_SANDBOX_SECCOMP_BPF_LINUX_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace sandbox {
class SandboxBPFPolicy;
}

namespace content {

class SandboxSeccompBPF {
 public:
  
  
  
  
  
  static bool IsSeccompBPFDesired();
  
  static bool ShouldEnableSeccompBPF(const std::string& process_type);
  
  
  static bool SupportsSandbox();
  
  
  static bool StartSandbox(const std::string& process_type);

  
  
  static bool StartSandboxWithExternalPolicy(
      scoped_ptr<sandbox::SandboxBPFPolicy> policy);
  
  static scoped_ptr<sandbox::SandboxBPFPolicy> GetBaselinePolicy();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SandboxSeccompBPF);
};

}  

#endif  

