// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_DELEGATE_EXECUTE_CRASH_SERVER_INIT_H_
#define WIN8_DELEGATE_EXECUTE_CRASH_SERVER_INIT_H_

#include "base/memory/scoped_ptr.h"

namespace google_breakpad {
class ExceptionHandler;
}

namespace delegate_execute {

scoped_ptr<google_breakpad::ExceptionHandler> InitializeCrashReporting();

}  

#endif  
