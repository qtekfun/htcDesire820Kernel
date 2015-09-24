// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_HELPERS_SYSCALL_PARAMETERS_RESTRICTIONS_H_
#define SANDBOX_LINUX_SECCOMP_BPF_HELPERS_SYSCALL_PARAMETERS_RESTRICTIONS_H_

#include "build/build_config.h"


namespace sandbox {

class ErrorCode;
class SandboxBPF;

ErrorCode RestrictCloneToThreadsAndEPERMFork(SandboxBPF* sandbox);

ErrorCode RestrictPrctl(SandboxBPF* sandbox);

ErrorCode RestrictIoctl(SandboxBPF* sandbox);

ErrorCode RestrictMmapFlags(SandboxBPF* sandbox);

ErrorCode RestrictMprotectFlags(SandboxBPF* sandbox);

ErrorCode RestrictFcntlCommands(SandboxBPF* sandbox);

#if defined(__i386__)
ErrorCode RestrictSocketcallCommand(SandboxBPF* sandbox);
#endif

}  

#endif  
