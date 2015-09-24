// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SUID_SANDBOX_H_
#define SANDBOX_LINUX_SUID_SANDBOX_H_

#if defined(__cplusplus)
namespace sandbox {
#endif

static const char kAdjustOOMScoreSwitch[] = "--adjust-oom-score";

static const char kSandboxDescriptorEnvironmentVarName[] = "SBX_D";
static const char kSandboxHelperPidEnvironmentVarName[] = "SBX_HELPER_PID";

static const long kSUIDSandboxApiNumber = 1;
static const char kSandboxEnvironmentApiRequest[] = "SBX_CHROME_API_RQ";
static const char kSandboxEnvironmentApiProvides[] = "SBX_CHROME_API_PRV";

static const int kZygoteIdFd = 7;

static const char kMsgChrootMe = 'C';
static const char kMsgChrootSuccessful = 'O';

static const char kSandboxPIDNSEnvironmentVarName[] = "SBX_PID_NS";
static const char kSandboxNETNSEnvironmentVarName[] = "SBX_NET_NS";

#if defined(__cplusplus)
}  
#endif

#endif  
