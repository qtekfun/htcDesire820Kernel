// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_DEBUG_DEBUGGER_H
#define BASE_DEBUG_DEBUGGER_H
#pragma once

#include "base/base_api.h"

namespace base {
namespace debug {

BASE_API bool SpawnDebuggerOnProcess(unsigned process_id);

BASE_API bool WaitForDebugger(int wait_seconds, bool silent);

BASE_API bool BeingDebugged();

BASE_API void BreakDebugger();

BASE_API void SetSuppressDebugUI(bool suppress);
BASE_API bool IsDebugUISuppressed();

}  
}  

#endif  
