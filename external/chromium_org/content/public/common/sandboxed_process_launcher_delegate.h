// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_SANDBOXED_PROCESS_LAUNCHER_DELEGATE_H_
#define CONTENT_PUBLIC_COMMON_SANDBOXED_PROCESS_LAUNCHER_DELEGATE_H_

#include "base/process/process.h"

namespace base {
class FilePath;
}

namespace sandbox {
class TargetPolicy;
}

namespace content {

class SandboxedProcessLauncherDelegate {
 public:
  virtual ~SandboxedProcessLauncherDelegate() {}

  
  
  
  virtual void ShouldSandbox(bool* in_sandbox) {}

  
  
  
  
  virtual void PreSandbox(bool* disable_default_policy,
                          base::FilePath* exposed_dir) {}

  
  virtual void PreSpawnTarget(sandbox::TargetPolicy* policy,
                              bool* success) {}

  
  virtual void PostSpawnTarget(base::ProcessHandle process) {}
};

}  

#endif  
