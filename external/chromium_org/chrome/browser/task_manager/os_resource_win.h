// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_OS_RESOURCE_WIN_H_
#define CHROME_BROWSER_TASK_MANAGER_OS_RESOURCE_WIN_H_

#include "base/process/process_handle.h"

namespace task_manager {

void GetWinGDIHandles(base::ProcessHandle process,
                      size_t* current,
                      size_t* peak);

void GetWinUSERHandles(base::ProcessHandle process,
                       size_t* current,
                       size_t* peak);

}  

#endif  
