// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_SANDBOX_INIT_H_
#define CONTENT_PUBLIC_COMMON_SANDBOX_INIT_H_

#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "build/build_config.h"
#include "content/common/content_export.h"

class CommandLine;

namespace base {
class FilePath;
}

namespace sandbox {
class SandboxBPFPolicy;
struct SandboxInterfaceInfo;
}

namespace content {
class SandboxedProcessLauncherDelegate;

#if defined(OS_WIN)

CONTENT_EXPORT bool InitializeSandbox(
    sandbox::SandboxInterfaceInfo* sandbox_info);

CONTENT_EXPORT bool BrokerDuplicateHandle(HANDLE source_handle,
                                          DWORD target_process_id,
                                          HANDLE* target_handle,
                                          DWORD desired_access,
                                          DWORD options);

CONTENT_EXPORT bool BrokerAddTargetPeer(HANDLE peer_process);

CONTENT_EXPORT base::ProcessHandle StartSandboxedProcess(
    SandboxedProcessLauncherDelegate* delegate,
    CommandLine* cmd_line);

#elif defined(OS_MACOSX)

CONTENT_EXPORT bool InitializeSandbox(int sandbox_type,
                                      const base::FilePath& allowed_path);

#elif defined(OS_LINUX)

class SandboxInitializerDelegate;

CONTENT_EXPORT bool InitializeSandbox(
    scoped_ptr<sandbox::SandboxBPFPolicy> policy);

CONTENT_EXPORT scoped_ptr<sandbox::SandboxBPFPolicy>
GetBPFSandboxBaselinePolicy();
#endif  

}  

#endif  
