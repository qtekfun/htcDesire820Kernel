// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SUID_SETUID_SANDBOX_CLIENT_H_
#define SANDBOX_LINUX_SUID_SETUID_SANDBOX_CLIENT_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"

namespace base { class Environment; }

namespace sandbox {

class SetuidSandboxClient {
 public:
  
  static class SetuidSandboxClient* Create();
  ~SetuidSandboxClient();

  
  
  
  bool ChrootMe();
  
  
  
  
  bool CreateInitProcessReaper(base::Closure* post_fork_parent_callback);

  
  bool IsSuidSandboxUpToDate() const;
  
  bool IsSuidSandboxChild() const;
  
  bool IsInNewPIDNamespace() const;
  
  bool IsInNewNETNamespace() const;
  
  bool IsSandboxed() const;

  
  
  void SetupLaunchEnvironment();

 private:
  
  base::Environment* env_;
  bool sandboxed_;
  DISALLOW_IMPLICIT_CONSTRUCTORS(SetuidSandboxClient);
};

}  

#endif  

