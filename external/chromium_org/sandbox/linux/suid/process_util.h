// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SANDBOX_LINUX_SUID_PROCESS_UTIL_H_
#define SANDBOX_LINUX_SUID_PROCESS_UTIL_H_

#include <stdbool.h>
#include <sys/types.h>

#include "base/base_export.h"

BASE_EXPORT bool AdjustOOMScore(pid_t process, int score);

BASE_EXPORT bool AdjustLowMemoryMargin(int64_t margin_mb);

#endif  
