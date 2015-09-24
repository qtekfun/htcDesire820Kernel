// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SANDBOX_UTIL_H_
#define CONTENT_COMMON_SANDBOX_UTIL_H_

#include "base/process/process.h"
#include "ipc/ipc_platform_file.h"


namespace content {

IPC::PlatformFileForTransit BrokerGetFileHandleForProcess(
    base::PlatformFile handle,
    base::ProcessId target_process_id,
    bool should_close_source);

}  

#endif  
