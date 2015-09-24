// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_SANDBOX_LINUX_H_
#define CONTENT_PUBLIC_COMMON_SANDBOX_LINUX_H_

namespace content {

enum LinuxSandboxStatus {
  
  kSandboxLinuxSUID = 1 << 0,

  
  kSandboxLinuxPIDNS = 1 << 1,

  
  kSandboxLinuxNetNS = 1 << 2,

  
  kSandboxLinuxSeccompBPF = 1 << 3,

  
  kSandboxLinuxInvalid = 1 << 31,
};

}  

#endif  
