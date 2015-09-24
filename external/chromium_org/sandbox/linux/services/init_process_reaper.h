// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SERVICES_INIT_PROCESS_REAPER_H_
#define SANDBOX_LINUX_SERVICES_INIT_PROCESS_REAPER_H_

#include "base/callback_forward.h"

namespace sandbox {

bool CreateInitProcessReaper(base::Closure* post_fork_parent_callback);

}  

#endif  
