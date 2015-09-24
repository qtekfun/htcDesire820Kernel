// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_HELPERS_SIGSYS_HANDLERS_H_
#define SANDBOX_LINUX_SECCOMP_BPF_HELPERS_SIGSYS_HANDLERS_H_

#include "base/basictypes.h"
#include "build/build_config.h"


namespace sandbox {

struct arch_seccomp_data;

intptr_t CrashSIGSYS_Handler(const struct arch_seccomp_data& args, void* aux);


intptr_t SIGSYSCloneFailure(const struct arch_seccomp_data& args, void* aux);
intptr_t SIGSYSPrctlFailure(const struct arch_seccomp_data& args, void* aux);
intptr_t SIGSYSIoctlFailure(const struct arch_seccomp_data& args, void* aux);

}  

#endif  
