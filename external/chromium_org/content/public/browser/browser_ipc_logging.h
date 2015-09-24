// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_IPC_LOGGING_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_IPC_LOGGING_H_

#include "content/common/content_export.h"
#include "ipc/ipc_message.h"  

namespace content {

#if defined(IPC_MESSAGE_LOG_ENABLED)

CONTENT_EXPORT void EnableIPCLogging(bool enable);

#endif

}  

#endif  
