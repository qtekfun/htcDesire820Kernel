// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SRC_WIN_PROCESS_MITIGATIONS_H_
#define SANDBOX_SRC_WIN_PROCESS_MITIGATIONS_H_

#include <windows.h>

#include "base/basictypes.h"
#include "sandbox/win/src/security_level.h"

namespace sandbox {

bool ApplyProcessMitigationsToCurrentProcess(MitigationFlags flags);

MitigationFlags FilterPostStartupProcessMitigations(MitigationFlags flags);

void ConvertProcessMitigationsToPolicy(MitigationFlags flags,
                                       DWORD64* policy_flags, size_t* size);

bool ApplyProcessMitigationsToSuspendedProcess(HANDLE process,
                                               MitigationFlags flags);

bool CanSetProcessMitigationsPostStartup(MitigationFlags flags);

bool CanSetProcessMitigationsPreStartup(MitigationFlags flags);

}  

#endif  

