// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_APP_STARTUP_HELPER_WIN_H_
#define CONTENT_PUBLIC_APP_STARTUP_HELPER_WIN_H_

class CommandLine;

namespace sandbox {
struct SandboxInterfaceInfo;
}

namespace content {

void InitializeSandboxInfo(sandbox::SandboxInterfaceInfo* sandbox_info);

void RegisterInvalidParamHandler();

void SetupCRT(const CommandLine& command_line);

}  

#endif  
