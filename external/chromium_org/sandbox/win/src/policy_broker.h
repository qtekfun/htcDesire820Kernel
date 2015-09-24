// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SRC_POLICY_BROKER_H_
#define SANDBOX_SRC_POLICY_BROKER_H_

#include "sandbox/win/src/interception.h"

namespace sandbox {

class TargetProcess;

bool SetupBasicInterceptions(InterceptionManager* manager);

bool SetupNtdllImports(TargetProcess *child);

}  

#endif  
