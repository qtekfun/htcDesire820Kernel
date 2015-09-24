// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_DUMP_WITHOUT_CRASHING_H_
#define CHROME_COMMON_DUMP_WITHOUT_CRASHING_H_

#include "build/build_config.h"

namespace logging {

void DumpWithoutCrashing();

#if defined(OS_POSIX)
void SetDumpWithoutCrashingFunction(void (*function)());
#endif

}  

#endif  
