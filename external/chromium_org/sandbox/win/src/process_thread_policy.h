// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SRC_PROCESS_THREAD_POLICY_H_
#define SANDBOX_SRC_PROCESS_THREAD_POLICY_H_

#include <string>

#include "sandbox/win/src/policy_low_level.h"

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "sandbox/win/src/crosscall_server.h"
#include "sandbox/win/src/sandbox_policy.h"

namespace sandbox {

enum EvalResult;

class ProcessPolicy {
 public:
  
  
  
  
  
  static bool GenerateRules(const wchar_t* name,
                            TargetPolicy::Semantics semantics,
                            LowLevelPolicy* policy);

  
  
  
  
  
  static NTSTATUS OpenThreadAction(const ClientInfo& client_info,
                                   uint32 desired_access,
                                   uint32 thread_id,
                                   HANDLE* handle);

  
  
  
  static NTSTATUS OpenProcessAction(const ClientInfo& client_info,
                                    uint32 desired_access,
                                    uint32 process_id,
                                    HANDLE* handle);

  
  
  
  static NTSTATUS OpenProcessTokenAction(const ClientInfo& client_info,
                                         HANDLE process,
                                         uint32 desired_access,
                                         HANDLE* handle);

  
  
  
  static NTSTATUS OpenProcessTokenExAction(const ClientInfo& client_info,
                                           HANDLE process,
                                           uint32 desired_access,
                                           uint32 attributes,
                                           HANDLE* handle);

  
  
  
  
  
  static DWORD CreateProcessWAction(EvalResult eval_result,
                                    const ClientInfo& client_info,
                                    const base::string16 &app_name,
                                    const base::string16 &command_line,
                                    PROCESS_INFORMATION* process_info);
};

}  


#endif  