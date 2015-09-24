// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SANDBOX_LINUX_SANDBOX_LINUX_H_
#define CONTENT_COMMON_SANDBOX_LINUX_SANDBOX_LINUX_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/common/sandbox_linux.h"

template <typename T> struct DefaultSingletonTraits;
namespace base {
class Thread;
}
namespace sandbox { class SetuidSandboxClient; }

namespace content {

class LinuxSandbox {
 public:
  
  
  
  
  enum LinuxSandboxIPCMethods {
    METHOD_GET_FONT_FAMILY_FOR_CHAR = 32,
    METHOD_LOCALTIME = 33,
    METHOD_GET_CHILD_WITH_INODE = 34,
    METHOD_GET_STYLE_FOR_STRIKE = 35,
    METHOD_MAKE_SHARED_MEMORY_SEGMENT = 36,
    METHOD_MATCH_WITH_FALLBACK = 37,
  };

  
  static LinuxSandbox* GetInstance();

  
  
  
  void PreinitializeSandbox();

  
  
  
  
  
  static bool InitializeSandbox();

  
  static void StopThread(base::Thread* thread);

  
  
  
  
  
  
  int GetStatus();
  
  
  bool IsSingleThreaded() const;
  
  bool seccomp_bpf_started() const;

  
  
  
  
  sandbox::SetuidSandboxClient* setuid_sandbox_client() const;

  
  
  
  bool StartSeccompBPF(const std::string& process_type);

  
  
  bool LimitAddressSpace(const std::string& process_type);

 private:
  friend struct DefaultSingletonTraits<LinuxSandbox>;

  
  
  bool InitializeSandboxImpl();
  void StopThreadImpl(base::Thread* thread);
  
  bool seccomp_bpf_supported() const;
  
  
  
  bool HasOpenDirectories() const;
  
  
  void SealSandbox();
  
  
  void CheckForBrokenPromises(const std::string& process_type);
  
  
  void StopThreadAndEnsureNotCounted(base::Thread* thread) const;

  
  
  
  int proc_fd_;
  bool seccomp_bpf_started_;
  
  int sandbox_status_flags_;
  
  bool pre_initialized_;
  bool seccomp_bpf_supported_;  
  scoped_ptr<sandbox::SetuidSandboxClient> setuid_sandbox_client_;

  ~LinuxSandbox();
  DISALLOW_IMPLICIT_CONSTRUCTORS(LinuxSandbox);
};

}  

#endif  
