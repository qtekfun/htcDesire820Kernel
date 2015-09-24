// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/webplugininfo.h"
#include "ipc/ipc_message_macros.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START UtilityMsgStart


IPC_MESSAGE_CONTROL0(UtilityMsg_BatchMode_Started)

IPC_MESSAGE_CONTROL0(UtilityMsg_BatchMode_Finished)

#if defined(OS_POSIX)
IPC_MESSAGE_CONTROL1(UtilityMsg_LoadPlugins,
                     std::vector<base::FilePath> )
#endif


#if defined(OS_POSIX)
IPC_SYNC_MESSAGE_CONTROL2_0(UtilityHostMsg_LoadPluginFailed,
                            uint32_t ,
                            base::FilePath )

IPC_SYNC_MESSAGE_CONTROL2_0(UtilityHostMsg_LoadedPlugin,
                            uint32_t ,
                            content::WebPluginInfo )
#endif  
