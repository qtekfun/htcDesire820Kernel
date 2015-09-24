// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SRC_REGISTRY_DISPATCHER_H_
#define SANDBOX_SRC_REGISTRY_DISPATCHER_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "sandbox/win/src/crosscall_server.h"
#include "sandbox/win/src/sandbox_policy_base.h"

namespace sandbox {

class RegistryDispatcher : public Dispatcher {
 public:
  explicit RegistryDispatcher(PolicyBase* policy_base);
  ~RegistryDispatcher() {}

  
  virtual bool SetupService(InterceptionManager* manager, int service);

 private:
  
  bool NtCreateKey(IPCInfo* ipc, base::string16* name, DWORD attributes,
                   HANDLE root, DWORD desired_access,
                   DWORD title_index, DWORD create_options);

  
  bool NtOpenKey(IPCInfo* ipc, base::string16* name, DWORD attributes,
                 HANDLE root, DWORD desired_access);

  PolicyBase* policy_base_;
  DISALLOW_COPY_AND_ASSIGN(RegistryDispatcher);
};

}  

#endif  